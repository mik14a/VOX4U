// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "VoxelSceneProxy.h"
#include "VoxelComponent.h"
#include "DynamicMeshBuilder.h"
#include "Voxel.h"

void FVoxelVertexBuffer::InitRHI()
{
	FRHIResourceCreateInfo CreateInfo;
	void* VertexBufferData = nullptr;
	VertexBufferRHI = RHIC8reateAndLockVertexBuffer(Vertices.Num() * sizeof(FDynamicMeshVertex), BUF_Static, CreateInfo, VertexBufferData);
	FMemory::Memcpy(VertexBufferData, Vertices.GetData(), Vertices.Num() * sizeof(FDynamicMeshVertex));
	RHIUnlockVertexBuffer(VertexBufferRHI);
}

void FVoxelIndexBuffer::InitRHI()
{
	FRHIResourceCreateInfo CreateInfo;
	void* Buffer = nullptr;
	IndexBufferRHI = RHICreateAndLockIndexBuffer(sizeof(int32), Indices.Num() * sizeof(int32), BUF_Static, CreateInfo, Buffer);
	FMemory::Memcpy(Buffer, Indices.GetData(), Indices.Num() * sizeof(int32));
	RHIUnlockIndexBuffer(IndexBufferRHI);
}

void FVoxelVertexFactory::Init(const FVoxelVertexBuffer* VertexBuffer)
{
	if (IsInRenderingThread()) {
		Init_RenderThread(VertexBuffer);
	} else {
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			InitVoxelVertexFactory,
			FVoxelVertexFactory*, VertexFactory, this,
			const FVoxelVertexBuffer*, VertexBuffer, VertexBuffer, {
				VertexFactory->Init_RenderThread(VertexBuffer);
			}
		);
	}
}

void FVoxelVertexFactory::Init_RenderThread(const FVoxelVertexBuffer* VertexBuffer)
{
	check(IsInRenderingThread());
	DataType Data;
	Data.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Position, VET_Float3);
	Data.TextureCoordinates.Add(FVertexStreamComponent(VertexBuffer, STRUCT_OFFSET(FDynamicMeshVertex, TextureCoordinate), sizeof(FDynamicMeshVertex), VET_Float2));
	Data.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentX, VET_PackedNormal);
	Data.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentZ, VET_PackedNormal);
	Data.ColorComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Color, VET_Color);
	SetData(Data);
}

FVoxelSceneProxy::FVoxelSceneProxy(UVoxelComponent* Component)
	: FPrimitiveSceneProxy(Component)
	, MaterialRelevance(Component->GetMaterialRelevance(GetScene().GetFeatureLevel()))
{
	Voxel = Component->GetVoxel();

	static const FIntVector Facees[] = {
		FIntVector( 0, 0, 1),	// Up
		FIntVector( 0, 0,-1),	// Down
		FIntVector( 1, 0, 0),	// Forward
		FIntVector(-1, 0, 0),	// Backward
		FIntVector( 0, 1, 0),	// Right
		FIntVector( 0,-1, 0)	// Left
	};
	static const FVector Vertexes[8] = {
		FVector(0, 0, 0),
		FVector(1, 0, 0),
		FVector(1, 1, 0),
		FVector(0, 1, 0),
		FVector(0, 0, 1),
		FVector(1, 0, 1),
		FVector(1, 1, 1),
		FVector(0, 1, 1),
	};
	static const uint32 Faces[6][2][3] = {
		{{ 5, 4, 7 }, { 7, 6, 5 }},	// Up 		FIntVector( 0, 0, 1)
		{{ 0, 1, 2 }, { 2, 3, 0 }},	// Down 	FIntVector( 0, 0,-1)
		{{ 5, 6, 2 }, { 2, 1, 5 }},	// Forward 	FIntVector( 1, 0, 0)
		{{ 7, 4, 0 }, { 0, 3, 7 }},	// Backward FIntVector(-1, 0, 0)
		{{ 6, 7, 3 }, { 3, 2, 6 }},	// Right 	FIntVector( 0, 1, 0)
		{{ 4, 5, 1 }, { 1, 0, 4 }}	// Left 	FIntVector( 0,-1, 0)
	};
	const FColor VertexColor(255, 255, 255);
	const FVector Offset = Voxel->bXYCenter ? FVector(-(float)Voxel->Size.X * 0.5f, -(float)Voxel->Size.Y * 0.5f, 0.f) : FVector::ZeroVector;
	for (auto Cell : Voxel->Voxel) {
		FVector Origin(Cell.Key.X, Cell.Key.Y, Cell.Key.Z);
		for (int FaceIndex = 0; FaceIndex < 6; ++FaceIndex) {
			if (Voxel->Voxel.Find(Cell.Key + Facees[FaceIndex])) continue;

			float TextureCoordinate = (float)Cell.Value / 256.f;
			for (int32 i = 0; i < 2; ++i) {
				FVector Vertex0 = Origin + Offset + Vertexes[Faces[FaceIndex][i][0]];
				FVector Vertex1 = Origin + Offset + Vertexes[Faces[FaceIndex][i][1]];
				FVector Vertex2 = Origin + Offset + Vertexes[Faces[FaceIndex][i][2]];

				const FVector Edge01 = (Vertex1 - Vertex0);
				const FVector Edge02 = (Vertex2 - Vertex0);

				const FVector TangentX = Edge01.GetSafeNormal();
				const FVector TangentZ = (Edge02 ^ Edge01).GetSafeNormal();
				const FVector TangentY = (TangentX ^ TangentZ).GetSafeNormal();

				FDynamicMeshVertex Vert0;
				Vert0.Position = Vertex0;
				Vert0.TextureCoordinate = FVector2D(TextureCoordinate, TextureCoordinate);
				Vert0.Color = VertexColor;
				Vert0.SetTangents(TangentX, TangentY, TangentZ);
				IndexBuffer.Indices.Add(VertexBuffer.Vertices.Add(Vert0));

				FDynamicMeshVertex Vert1;
				Vert1.Position = Vertex1;
				Vert1.TextureCoordinate = FVector2D(TextureCoordinate, TextureCoordinate);
				Vert1.Color = VertexColor;
				Vert1.SetTangents(TangentX, TangentY, TangentZ);
				IndexBuffer.Indices.Add(VertexBuffer.Vertices.Add(Vert1));

				FDynamicMeshVertex Vert2;
				Vert2.Position = Vertex2;
				Vert2.TextureCoordinate = FVector2D(TextureCoordinate, TextureCoordinate);
				Vert2.Color = VertexColor;
				Vert2.SetTangents(TangentX, TangentY, TangentZ);
				IndexBuffer.Indices.Add(VertexBuffer.Vertices.Add(Vert2));
			}
		}
	}

	VertexFactory.Init(&VertexBuffer);
	BeginInitResource(&VertexBuffer);
	BeginInitResource(&IndexBuffer);
	BeginInitResource(&VertexFactory);

	Material = Component->GetMaterial(0);
	if (Material == NULL)
	{
		Material = UMaterial::GetDefaultMaterial(MD_Surface);
	}
}

FVoxelSceneProxy::~FVoxelSceneProxy()
{
	VertexBuffer.ReleaseResource();
	IndexBuffer.ReleaseResource();
	VertexFactory.ReleaseResource();
}

void FVoxelSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_VoxelSceneProxy_GetDynamicMeshElements);

	const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;
	FMaterialRenderProxy* WireframeMaterialInstance = new FColoredMaterialRenderProxy(GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy(IsSelected()) : NULL,	FLinearColor(0, 0.5f, 1.f));
	Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);
	FMaterialRenderProxy* MaterialProxy = bWireframe ? WireframeMaterialInstance : Material->GetRenderProxy(IsSelected());

	for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++) {
		if (VisibilityMap & (1 << ViewIndex)) {
			const FSceneView* View = Views[ViewIndex];

			FMeshBatch& MeshBatch = Collector.AllocateMesh();
			MeshBatch.bWireframe = bWireframe;
			MeshBatch.VertexFactory = &VertexFactory;
			MeshBatch.MaterialRenderProxy = MaterialProxy;
			MeshBatch.ReverseCulling = IsLocalToWorldDeterminantNegative();
			MeshBatch.Type = PT_TriangleList;
			MeshBatch.DepthPriorityGroup = SDPG_World;
			MeshBatch.bCanApplyViewModeOverrides = false;

			FMeshBatchElement& MeshBatchElement = MeshBatch.Elements[0];
			MeshBatchElement.IndexBuffer = &IndexBuffer;
			MeshBatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, UseEditorDepthTest());
			MeshBatchElement.FirstIndex = 0;
			MeshBatchElement.NumPrimitives = IndexBuffer.Indices.Num() / 3;
			MeshBatchElement.MinVertexIndex = 0;
			MeshBatchElement.MaxVertexIndex = VertexBuffer.Vertices.Num() - 1;

			Collector.AddMesh(ViewIndex, MeshBatch);
		}
	}
}

FPrimitiveViewRelevance FVoxelSceneProxy::GetViewRelevance(const FSceneView* View)
{
	FPrimitiveViewRelevance Result;
	Result.bDrawRelevance = IsShown(View);
	Result.bShadowRelevance = IsShadowCast(View);
	Result.bDynamicRelevance = true;
	Result.bRenderInMainPass = ShouldRenderInMainPass();
	Result.bRenderCustomDepth = ShouldRenderCustomDepth();
	MaterialRelevance.SetPrimitiveViewRelevance(Result);
	return Result;
}

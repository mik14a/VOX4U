// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "PrimitiveSceneProxy.h"
#include "RenderResource.h"
#include "VertexFactory.h"
#include "DynamicMeshBuilder.h"

class UVoxel;
class UMeshedVoxelComponent;

class FMeshedVoxelVertexBuffer : public FVertexBuffer
{
public:
	TArray<FDynamicMeshVertex> Vertices;
	virtual void InitRHI() override;
};

class FMeshedVoxelIndexBuffer : public FIndexBuffer
{
public:
	TArray<int32> Indices;
	virtual void InitRHI() override;
};

class FMeshedVoxelVertexFactory : public FLocalVertexFactory
{
public:
	FMeshedVoxelVertexFactory() {}
	void Init(const FMeshedVoxelVertexBuffer* VertexBuffer);
	void Init_RenderThread(const FMeshedVoxelVertexBuffer* VertexBuffer);
};

/**
 *
 */
class FMeshedVoxelSceneProxy : public FPrimitiveSceneProxy
{
public:
	FMeshedVoxelSceneProxy(UMeshedVoxelComponent* Component);

	virtual ~FMeshedVoxelSceneProxy();

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override;

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;

	virtual bool CanBeOccluded() const override { return !MaterialRelevance.bDisableDepthTest; }

	virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }

	uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

private:

	UVoxel* Voxel;
	UMaterialInterface* Material;
	FMeshedVoxelVertexBuffer VertexBuffer;
	FMeshedVoxelIndexBuffer IndexBuffer;
	FMeshedVoxelVertexFactory VertexFactory;
	FMaterialRelevance MaterialRelevance;
};

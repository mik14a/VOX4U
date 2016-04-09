// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "InstancedVoxelComponent.h"
#include "InstancedVoxel.h"
#include "GameFramework/Actor.h"

UInstancedVoxelComponent::UInstancedVoxelComponent()
	: CellBounds(FVector::ZeroVector, FVector(100.f, 100.f, 100.f), 100.f)
	, bHideUnbeheld(true)
	, Mesh()
	, Cell()
	, Voxel(nullptr)
	, InstancedStaticMeshComponents()
{
}

#if WITH_EDITOR
void UInstancedVoxelComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	static const FName NAME_HideUnbeheld = FName(TEXT("bHideUnbeheld"));
	static const FName NAME_Mesh = FName(TEXT("Mesh"));
	static const FName NAME_Voxel = FName(TEXT("Voxel"));
	if (PropertyChangedEvent.Property) {
		if (PropertyChangedEvent.Property->GetFName() == NAME_HideUnbeheld) {
			ClearVoxel();
			AddVoxel();
		} else if (PropertyChangedEvent.Property->GetFName() == NAME_Mesh) {
			FBoxSphereBounds Bounds(ForceInit);
			for (int32 i = 0; i < Mesh.Num(); ++i) {
				InstancedStaticMeshComponents[i]->SetStaticMesh(Mesh[i]);
				if (Mesh[i]) {
					Bounds = Bounds + Mesh[i]->GetBounds();
				}
			}
			CellBounds = Bounds;
		} else if (PropertyChangedEvent.Property->GetFName() == NAME_Voxel) {
			SetVoxel(Voxel, true);
		}
	}
}
#endif // WITH_EDITOR

void UInstancedVoxelComponent::SetVoxel(class UInstancedVoxel* InVoxel, bool bForce /*= false*/)
{
	if (Voxel != InVoxel || bForce) {
		Voxel = InVoxel;
		InitVoxel();
	}
}

void UInstancedVoxelComponent::InitVoxel()
{
	CellBounds = FBoxSphereBounds(FVector::ZeroVector, FVector(100.f, 100.f, 100.f), 100.f);
	Mesh.Empty();
	Cell.Empty();
	InstancedStaticMeshComponents.Empty();
	if (Voxel) {
		CellBounds = Voxel->CellBounds;
		Mesh = Voxel->Mesh;
		Cell = Voxel->Voxel;
		for (int32 i = 0; i < Mesh.Num(); ++i) {
			UInstancedStaticMeshComponent* Proxy = NewObject<UInstancedStaticMeshComponent>(this, NAME_None, RF_Transactional);
			Proxy->SetStaticMesh(Mesh[i]);
			Proxy->AttachTo(GetOwner()->GetRootComponent(), NAME_None);
			InstancedStaticMeshComponents.Add(Proxy);
		}
		AddVoxel();
	}
}

void UInstancedVoxelComponent::AddVoxel()
{
	FVector Offset = Voxel->bXYCenter ? FVector((float)Voxel->Size.X, (float)Voxel->Size.Y, 0.f) * CellBounds.BoxExtent : FVector::ZeroVector;
	for (auto& voxel : Voxel->Voxel) {
		if (bHideUnbeheld && IsUnbeheldVolume(voxel)) continue;
		FVector Translation = FVector(voxel) * CellBounds.BoxExtent * 2 -CellBounds.Origin + CellBounds.BoxExtent - Offset;
		FTransform Transform(FQuat::Identity, Translation, FVector(1.f));
		InstancedStaticMeshComponents[voxel.I]->AddInstance(Transform);
	}
}

void UInstancedVoxelComponent::ClearVoxel()
{
	for (int32 i = 0; i < Mesh.Num(); ++i) {
		InstancedStaticMeshComponents[i]->ClearInstances();
	}
}

bool UInstancedVoxelComponent::IsUnbeheldVolume(const FIntVector& InVector) const
{
	static const TArray<FIntVector> Direction = TArrayBuilder<FIntVector>()
		.Add(FIntVector(+0, +0, +1))	// Up
		.Add(FIntVector(+0, +0, -1))	// Down
		.Add(FIntVector(+1, +0, +0))	// Forward
		.Add(FIntVector(-1, +0, +0))	// Backward
		.Add(FIntVector(+0, +1, +0))	// Right
		.Add(FIntVector(+0, -1, +0));	// Left
	int count = 0;
	for (int i = 0; i < Direction.Num(); ++i) {
		if (INDEX_NONE != Voxel->Voxel.IndexOfByKey(InVector + Direction[i])) {
			++count;
		}
	}
	return Direction.Num() == count;
}

bool UInstancedVoxelComponent::GetVoxelTransform(int32 VoxelIndex, FTransform& OutVoxelTransform, bool bWorldSpace /*= false*/) const
{
	if (!Cell.IsValidIndex(VoxelIndex)) return false;
	FVector Offset = Voxel->bXYCenter ? FVector((float)Voxel->Size.X, (float)Voxel->Size.Y, 0.f) * CellBounds.BoxExtent : FVector::ZeroVector;
	FVector Translation = FVector(Cell[VoxelIndex]) * CellBounds.BoxExtent * 2 - CellBounds.Origin + CellBounds.BoxExtent - Offset;
	OutVoxelTransform = FTransform(FQuat::Identity, Translation, FVector(1.f));
	if (bWorldSpace) {
		OutVoxelTransform = OutVoxelTransform * ComponentToWorld;
	}
	return true;
}

const TArray<UInstancedStaticMeshComponent*>& UInstancedVoxelComponent::GetInstancedStaticMeshComponent() const
{
	return InstancedStaticMeshComponents;
}

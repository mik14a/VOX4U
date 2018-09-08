// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#include "VoxelComponent.h"
#include <Components/InstancedStaticMeshComponent.h>
#include <Containers/ArrayBuilder.h>
#include <Engine/StaticMesh.h>
#include "Voxel.h"

UVoxelComponent::UVoxelComponent()
	: CellBounds(FVector::ZeroVector, FVector(100.f, 100.f, 100.f), 100.f)
	, bHideUnbeheld(true)
	, Mesh()
	, Cell()
	, Voxel(nullptr)
	, InstancedStaticMeshComponents()
{
}

#if WITH_EDITOR
void UVoxelComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
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
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

void UVoxelComponent::SetVoxel(class UVoxel* InVoxel, bool bForce /*= false*/)
{
	if (Voxel != InVoxel || bForce) {
		Voxel = InVoxel;
		InitVoxel();
	}
}

const UVoxel* UVoxelComponent::GetVoxel() const
{
	return Voxel;
}

void UVoxelComponent::InitVoxel()
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
			Proxy->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
			InstancedStaticMeshComponents.Add(Proxy);
		}
		AddVoxel();
	}
}

void UVoxelComponent::AddVoxel()
{
	FVector Offset = Voxel->bXYCenter ? FVector((float)Voxel->Size.X, (float)Voxel->Size.Y, 0.f) * CellBounds.BoxExtent : FVector::ZeroVector;
	for (auto& voxel : Voxel->Voxel) {
		if (bHideUnbeheld && IsUnbeheldVolume(voxel.Key)) continue;
		FVector Translation = FVector(voxel.Key) * CellBounds.BoxExtent * 2 -CellBounds.Origin + CellBounds.BoxExtent - Offset;
		FTransform Transform(FQuat::Identity, Translation, FVector(1.f));
		InstancedStaticMeshComponents[voxel.Value]->AddInstance(Transform);
	}
}

void UVoxelComponent::ClearVoxel()
{
	for (int32 i = 0; i < Mesh.Num(); ++i) {
		InstancedStaticMeshComponents[i]->ClearInstances();
	}
}

bool UVoxelComponent::IsUnbeheldVolume(const FIntVector& InVector) const
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
		if (Voxel->Voxel.Contains(InVector + Direction[i])) {
			++count;
		}
	}
	return Direction.Num() == count;
}

bool UVoxelComponent::GetVoxelTransform(const FIntVector& InVector, FTransform& OutVoxelTransform, bool bWorldSpace /*= false*/) const
{
	if (!Cell.Contains(InVector)) return false;
	FVector Offset = Voxel->bXYCenter ? FVector((float)Voxel->Size.X, (float)Voxel->Size.Y, 0.f) * CellBounds.BoxExtent : FVector::ZeroVector;
	FVector Translation = FVector(InVector) * CellBounds.BoxExtent * 2 - CellBounds.Origin + CellBounds.BoxExtent - Offset;
	OutVoxelTransform = FTransform(FQuat::Identity, Translation, FVector(1.f));
	if (bWorldSpace) {
		OutVoxelTransform = OutVoxelTransform * GetComponentToWorld();
	}
	return true;
}

FBoxSphereBounds UVoxelComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBoxSphereBounds Bounds = FBoxSphereBounds(ForceInit);
	for (auto* InstancedStaticMeshComponent : InstancedStaticMeshComponents) {
		Bounds = Bounds + InstancedStaticMeshComponent->CalcBounds(LocalToWorld);
	}
	return Bounds;
}

const TArray<UInstancedStaticMeshComponent*>& UVoxelComponent::GetInstancedStaticMeshComponent() const
{
	return InstancedStaticMeshComponents;
}

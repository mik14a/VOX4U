// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "VoxelComponent.h"
#include "Voxel.h"
#include "ArrayBuilder.h"

#if WITH_EDITOR

void UVoxelComponent::BeginPlay()
{

}

void UVoxelComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	static const FName NAME_Voxel = FName(TEXT("Voxel"));

	if (PropertyChangedEvent.Property) {
		if (PropertyChangedEvent.Property->GetFName() == NAME_Voxel) {
			ClearInstances();
			if (Voxel) {
				AddVoxelWorldSpace();
			}
		}
	}
}

void UVoxelComponent::AddVoxelWorldSpace()
{
	FVector Offset = Voxel->bXYCenter ? FVector((float)Voxel->Size.X / 2, (float)Voxel->Size.Y / 2, 0.f) : FVector::ZeroVector;
	for (auto& voxel : Voxel->Voxel) {
		if (IsUnbeheldVolume(voxel.Key)) continue;
		FVector Translation = FVector(voxel.Key.X, voxel.Key.Y, voxel.Key.Z) - Offset;
		FTransform Transform(FQuat::Identity, Translation, FVector(1.f));
		AddInstance(Transform);
	}
}

bool UVoxelComponent::IsUnbeheldVolume(const FIntVector& Cell) const
{
	static const TArray<FIntVector> Direction = TArrayBuilder<FIntVector>()
		.Add(FIntVector(0, 0, 1))	// Up
		.Add(FIntVector(0, 0, -1))	// Down
		.Add(FIntVector(1, 0, 0))	// Forward
		.Add(FIntVector(-1, 0, 0))	// Backward
		.Add(FIntVector(0, 1, 0))	// Right
		.Add(FIntVector(0, -1, 0));	// Left
	return Direction.FilterByPredicate([&](const FIntVector& direction) {
		return Voxel->Voxel.Find(Cell + direction);
	}).Num() == Direction.Num();
}

#endif // WITH_EDITOR

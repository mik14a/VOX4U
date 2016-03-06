// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "Voxel.h"

UVoxel::UVoxel()
	: Size(ForceInit)
	, bXYCenter(true)
	, Material(nullptr)
{
}

void UVoxel::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	if (Ar.IsSaving()) {
		int32 NumVoxels = Voxel.Num();
		Ar << NumVoxels;
		for (auto& voxel : Voxel) {
			Ar << voxel.Key.X << voxel.Key.Y << voxel.Key.Z << voxel.Value;
		}
	} else if (Ar.IsLoading()) {
		int32 NumVoxels;
		Ar << NumVoxels;
		for (int32 i = 0; i < NumVoxels; ++i) {
			FIntVector voxel;
			uint8 index;
			Ar << voxel.X << voxel.Y << voxel.Z << index;
			Voxel.FindOrAdd(voxel) = index;
		}
	}
}

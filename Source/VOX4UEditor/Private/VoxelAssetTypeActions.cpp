// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#include "VoxelAssetTypeActions.h"
#include "Voxel.h"

FVoxelAssetTypeActions::FVoxelAssetTypeActions()
{
}

FText FVoxelAssetTypeActions::GetName() const
{
	return NSLOCTEXT("VOX4U", "VoxelAssetTypeActionsName", "Voxel Asset Type Actions.");
}

UClass* FVoxelAssetTypeActions::GetSupportedClass() const
{
	return UVoxel::StaticClass();
}

FColor FVoxelAssetTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 FVoxelAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

bool FVoxelAssetTypeActions::IsImportedAsset() const
{
	return true;
}

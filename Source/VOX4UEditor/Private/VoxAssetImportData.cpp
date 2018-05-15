// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#include "VoxAssetImportData.h"

UVoxAssetImportData::UVoxAssetImportData()
	: VoxImportType(EVoxImportType::StaticMesh)
	, bImportXForward(true)
	, bImportXYCenter(true)
	, Scale(10.f)
{
}

void UVoxAssetImportData::ToVoxImportOption(UVoxImportOption& OutVoxImportOption)
{
	OutVoxImportOption.VoxImportType = VoxImportType;
	OutVoxImportOption.bImportXForward = bImportXForward;
	OutVoxImportOption.bImportXYCenter = bImportXYCenter;
	OutVoxImportOption.Scale = Scale;
	OutVoxImportOption.BuildSettings.BuildScale3D = FVector(Scale);
}

void UVoxAssetImportData::FromVoxImportOption(const UVoxImportOption& VoxImportOption)
{
	VoxImportType = VoxImportOption.VoxImportType;
	bImportXForward = VoxImportOption.bImportXForward;
	bImportXYCenter = VoxImportOption.bImportXYCenter;
	Scale = VoxImportOption.Scale;
}

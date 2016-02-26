// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "VoxImportOption.h"

UVoxImportOption::UVoxImportOption()
	: VoxImportType(EVoxImportType::StaticMesh)
	, bImportXForward(true)
	, bImportXYCenter(true)
	, bMergeVertexes(true)
	, Material(nullptr)
	, Mesh(nullptr)
{
	BuildSettings.BuildScale3D = FVector(10.f);
}

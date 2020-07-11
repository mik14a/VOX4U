// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IVoxImporter.h"

/**
 * VoxImporter
 */
class VoxImporter : public IVoxImporter
{
public:
	/** Construct importer use parent voxel reference */
	VoxImporter(FVoxel* voxel);

	/** Import voxel model from vox object */
	void Import(const vox::vox& vox) override;
};

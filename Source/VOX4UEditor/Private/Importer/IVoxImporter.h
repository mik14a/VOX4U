// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FVoxel.h"
#include "VoxFormat/Public/Vox.h"

struct FVoxel;

/**
 * IVoxImporter
 */
struct IVoxImporter
{
public:
	IVoxImporter(FVoxel* voxel) : Voxel(voxel) {}
	virtual ~IVoxImporter() = default;

	/** Import voxel model from vox object */
	virtual void Import(const FVox& vox) = 0;

protected: // Immutable members
	/** Parent voxel structure */
	FVoxel* const Voxel;
};

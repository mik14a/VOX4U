// Copyright 2016-2023 mik14a / Admix Network. All Rights Reserved.

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

protected:

	/** Load palette from vox chunk */
	static void LoadPalette(const FVoxChunkRgba& Rgba, TArray<FColor>& OutPalette);

	/** Load material from vox chunk */
	static void LoadMaterial(const FVox::MaterialT& Materials, TArray<FVoxMaterial>& OutMaterials);
};

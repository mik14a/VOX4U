// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

struct FRawMesh;
struct FVoxel;
class UVoxImportOption;

/**
 * IMesher
 */
class IMesher
{
public:
	/** Construct mesh generator */
	IMesher(const FVoxel* InVox) : Vox(InVox) {}

	virtual ~IMesher() = default;

	/** Create FRawMesh from Voxel */
	virtual bool CreateRawMesh(FRawMesh& OutRawMesh, const UVoxImportOption* ImportOption) const = 0;

protected:  // Immutable members
	const FVoxel* const Vox;
};

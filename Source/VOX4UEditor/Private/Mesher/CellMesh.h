// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IMesher.h"

/**
 * CellMesh
 */
class CellMesh : public IMesher
{
public:
	/** Construct mesh generator */
	CellMesh(const FVoxel* InVox);

	/** Create FRawMesh from Voxel */
	bool CreateRawMesh(FRawMesh& OutRawMesh, const UVoxImportOption* ImportOption) const override;

	/** Create raw meshes from Voxel */
	bool CreateRawMeshes(TArray<FRawMesh>& OutRawMeshes, const UVoxImportOption* ImportOption) const;

	/** Create one raw mesh */
	static bool CreateBoxCell(FRawMesh& OutRawMesh, const UVoxImportOption* ImportOption);
};

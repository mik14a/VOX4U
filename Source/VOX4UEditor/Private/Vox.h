// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <RawMesh.h>

class UTexture2D;
class UVoxImportOption;

/**
 * @struct FVox
 * VOX format implementation.
 * @see https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt
 */
struct FVox
{
	/** Filename */
	FString Filename;

	/** Magic number ( 'V' 'O' 'X' 'space' ) and terminate */
	ANSICHAR MagicNumber[5];
	/** version number ( current version is 150 ) */
	uint32 VersionNumber;

	/** Size */
	FIntVector Size;
	/** Voxel */
	TMap<FIntVector, uint8> Voxel;
	/** Palette */
	TArray<FColor> Palette;

public:

	/** Create empty vox data */
	FVox();

	/** Create vox data from archive */
	FVox(const FString& Filename, FArchive& Ar, const UVoxImportOption* ImportOption);

	/** Import vox data from archive */
	bool Import(FArchive& Ar, const UVoxImportOption* ImportOption);

	/** Create FRawMesh from Voxel */
	bool CreateRawMesh(FRawMesh& OutRawMesh, const UVoxImportOption* ImportOption) const;

	/** Create FRawMesh from Voxel use Monotone mesh generation */
	bool CreateOptimizedRawMesh(FRawMesh& OutRawMesh, const UVoxImportOption* ImportOption) const;

	/** Create raw meshes from Voxel */
	bool CreateRawMeshes(TArray<FRawMesh>& OutRawMeshes, const UVoxImportOption* ImportOption) const;

	/** Create UTexture2D from Palette */
	bool CreateTexture(UTexture2D* const& OutTexture, UVoxImportOption* ImportOption) const;

	/** Create one raw mesh */
	static bool CreateMesh(FRawMesh& OutRawMesh, const UVoxImportOption* ImportOption);
};

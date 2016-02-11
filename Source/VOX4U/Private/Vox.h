// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "ArchiveBase.h"
#include "Vox.generated.h"

struct FRawMesh;

USTRUCT()
struct FCell
{
	GENERATED_BODY()

	uint8 X, Y, Z, I;
	FString ToString() {
		return FString::Printf(TEXT("X=%d Y=%d Z=%d I=%d"), X, Y, Z, I);
	}
};

/**
 * @struct FVox
 * VOX format implementation.
 * @see https://ephtracy.github.io/index.html?page=mv_vox_format
 */
USTRUCT()
struct FVox
{
	GENERATED_BODY()

	ANSICHAR MagicNumber[5];
	uint32 VersionNumber;

	FIntVector Size;
	TArray<FCell> Voxel;
	TArray<FColor> Palette;

public:

	/** Create empty vox data */
	FVox();

	/** Create vox data from archive */
	FVox(FArchive& Ar);

	/** Import vox data from archive */
	bool Import(FArchive& Ar);

	/** Create FRawMesh */
	bool CreateRawMesh(FRawMesh& RawMesh);

};

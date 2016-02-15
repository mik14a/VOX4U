// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "ArchiveBase.h"
#include "Vox.generated.h"

struct FRawMesh;
class UVoxImportOption;

USTRUCT()
struct FCell
{
	GENERATED_BODY()

	int8 X, Y, Z, I;

	FCell() { }

	FCell(int8 InX, int8 InY, int8 InZ) : X(InX), Y(InY), Z(InZ), I(0) { }

	FCell& operator+=(const FCell& Other) {
		X += Other.X;
		Y += Other.Y;
		Z += Other.Z;
		return *this;
	}

	FCell operator+(const FCell& Other) const {
		return FCell(*this) += Other;
	}

	FIntVector ToIntVector() const {
		return FIntVector(X, Y, Z);
	}

	FString ToString() const {
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
	FVox(FArchive& Ar, const UVoxImportOption* ImportOption);

	/** Import vox data from archive */
	bool Import(FArchive& Ar, const UVoxImportOption* ImportOption);

	/** Create FRawMesh */
	bool CreateRawMesh(FRawMesh& RawMesh, const UVoxImportOption* ImportOption) const;

};

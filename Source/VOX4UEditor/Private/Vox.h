// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Vox.generated.h"

struct FRawMesh;
class UVoxImportOption;
class UTexture2D;

/**
 * Cell struct for VOXEL chunk
 */
USTRUCT()
struct FCell
{
	GENERATED_BODY()

	/** X */
	int8 X;

	/** Y */
	int8 Y;

	/** Z */
	int8 Z;

	/** Color index */
	uint8 I;

	/** Default constructor */
	FCell() { }

	/**
	 * Construct with X, Y, Z, ColorIndex.
	 */
	FCell(int8 InX, int8 InY, int8 InZ, uint8 InI = 0) : X(InX), Y(InY), Z(InZ), I(InI) { }

	/** Add position to cell */
	FCell& operator+=(const FIntVector& Other) {
		X += Other.X;
		Y += Other.Y;
		Z += Other.Z;
		return *this;
	}

	/** Add position to cell */
	FCell operator+(const FIntVector& Other) const {
		return FCell(*this) += Other;
	}

	/** Convert to FIntVector */
	FIntVector ToIntVector() const {
		return FIntVector(X, Y, Z);
	}

	FORCEINLINE FString ToString() const {
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

	/** Magic number ( 'V' 'O' 'X' 'space' ) and terminate */
	ANSICHAR MagicNumber[5];
	/** version number ( current version is 150 ) */
	uint32 VersionNumber;

	/** Size */
	FIntVector Size;
	/** Voxel */
	TArray<FCell> Voxel;
	/** Palette */
	TArray<FColor> Palette;

public:

	/** Create empty vox data */
	FVox();

	/** Create vox data from archive */
	FVox(FArchive& Ar, const UVoxImportOption* ImportOption);

	/** Import vox data from archive */
	bool Import(FArchive& Ar, const UVoxImportOption* ImportOption);

	/** Create FRawMesh from Voxel */
	bool CreateRawMesh(FRawMesh& OutRawMesh, const UVoxImportOption* ImportOption) const;

	/** Create raw meshes from Voxel */
	bool CreateRawMeshes(TArray<FRawMesh>& OutRawMeshes, const UVoxImportOption* ImportOption) const;

	/** Create UTexture2D from Palette */
	bool CreateTexture(UTexture2D* const& OutTexture, UVoxImportOption* ImportOption) const;

	/** Create one raw mesh */
	static bool CreateMesh(FRawMesh& OutRawMesh, const UVoxImportOption* ImportOption);
};

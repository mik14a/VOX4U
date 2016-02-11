// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "Vox.h"
#include "RawMesh.h"

DEFINE_LOG_CATEGORY_STATIC(LogVox, Log, All)


/**
 * Create empty vox data.
 */
	FVox::FVox()
{
}

/**
 * Create vox data from archive
 * @param FArchive& Ar	Read vox data from the archive
 */
FVox::FVox(FArchive& Ar)
{
	Import(Ar);
}

/**
 * Import vox data from archive
 * @param FArchive& Ar	Read vox data from the archive
 * @return bool	is valid or supported vox data
 */
bool FVox::Import(FArchive & Ar)
{
	Ar.Serialize(MagicNumber, 4);

	if (0 != FCStringAnsi::Strncmp("VOX ", MagicNumber, 4)) {
		UE_LOG(LogVox, Error, TEXT("not a vox format"));
		return false;
	}
	UE_LOG(LogVox, Verbose, TEXT("MAGIC NUMBER: %s"), &MagicNumber);

	Ar << VersionNumber;
	UE_LOG(LogVox, Verbose, TEXT("VERSION NUMBER: %d"), VersionNumber);

	if (150 < VersionNumber) {
		UE_LOG(LogVox, Error, TEXT("unsupported version."));
		return false;
	}

	ANSICHAR ChunkId[5] = { 0, };
	uint32 SizeOfChunkContents;
	uint32 TotalSizeOfChildrenChunks;

	do {
		Ar.Serialize(ChunkId, 4);
		Ar << SizeOfChunkContents;
		Ar << TotalSizeOfChildrenChunks;
		if (0 == FCStringAnsi::Strncmp("MAIN", ChunkId, 4)) {
			UE_LOG(LogVox, Verbose, TEXT("MAIN: "));
		} else if (0 == FCStringAnsi::Strncmp("SIZE", ChunkId, 4)) {
			Ar << Size.X << Size.Y << Size.Z;
			UE_LOG(LogVox, Verbose, TEXT("SIZE: %s"), *Size.ToString());
		} else if (0 == FCStringAnsi::Strncmp("XYZI", ChunkId, 4)) {
			uint32 NumVoxels;
			Ar << NumVoxels;
			UE_LOG(LogVox, Verbose, TEXT("XYZI: NumVoxels=%d"), NumVoxels);
			FCell Cell;
			for (uint32 i = 0; i < NumVoxels; ++i) {
				Ar << Cell.X << Cell.Y << Cell.Z << Cell.I;
				UE_LOG(LogVox, Verbose, TEXT("      Voxel %s"), *Cell.ToString());
				Voxel.Add(Cell);
			}
		} else if (0 == FCStringAnsi::Strncmp("RGBA", ChunkId, 4)) {
			UE_LOG(LogVox, Verbose, TEXT("RGBA:"));
			FColor Color;
			for (uint32 i = 0; i < SizeOfChunkContents / 4; ++i) {
				Ar << Color.R << Color.G << Color.B << Color.A;
				UE_LOG(LogVox, Verbose, TEXT("      %s"), *Color.ToString());
				Palette.Add(Color);
			}
		} else {
			UE_LOG(LogVox, Error, TEXT("unknown chunk. [ %s ]"), &ChunkId);
		}
	} while (!Ar.AtEnd());

	return true;
}

/**
 * UE4
 * FVector::UpVector(0.0f, 0.0f, 1.0f);
 * FVector::ForwardVector(1.0f, 0.0f, 0.0f);
 * FVector::RightVector(0.0f, 1.0f, 0.0f);
 *
 *   z (up)
 *   |
 *   +---x (front)
 *  /
 * y (right)
 *
 * MagicaVoxel
 * FVector::UpVector(0.0f, 0.0f, 1.0f);
 * FVector::ForwardVector(0.0f, -1.0f, 0.0f);
 * FVector::RightVector(-1.0f, 0.0f, 0.0f);
 *
 *   z (up)
 *   |
 *   | y (back)
 *   |/
 *   +---x (left)
 *
 *   4---5
 *  /|  /|
 * 7---6 |
 * | 0-|-1
 * |/  |/
 * 3---2
 */
static const FVector Vertex[] = {
	FVector(0, 0, 0),
	FVector(0, 1, 0),
	FVector(1, 1, 0),
	FVector(1, 0, 0),
	FVector(0, 0, 1),
	FVector(0, 1, 1),
	FVector(1, 1, 1),
	FVector(1, 0, 1),
};
static const uint32 Index[] = {
	// 1,0,3,2
	1,0,3,
	3,2,1,
	// 4,5,6,7
	4,5,6,
	6,7,4,
	// 7,6,2,3
	7,6,2,
	2,3,7,
	// 6,5,1,2
	6,5,1,
	1,2,6,
	// 5,4,0,1
	5,4,0,
	0,1,5,
	// 4,7,3,0
	4,7,3,
	3,0,4,
};
static const FVector2D TexCoord[] = {
	FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1),
	FVector2D(1, 1), FVector2D(1, 0), FVector2D(0, 0),
};

/**
 * CreateRawMesh
 * @param FRawMesh& RawMesh	Out RawMesh
 * @return Result
 */
bool FVox::CreateRawMesh(FRawMesh& RawMesh)
{
	for (FCell cell : Voxel) {

		FVector Vector(cell.X, cell.Y, cell.Z);
		uint32 Position = RawMesh.VertexPositions.Num();

		TArray<FVector> Positions;
		for (int32 i = 0; i < 8; ++i) {
			Positions.Add(Vector + Vertex[i]);
		}
		RawMesh.VertexPositions.Append(Positions);

		TArray<uint32> Indices;
		for (int32 i = 0; i < 36; ++i) {
			Indices.Add(Position + Index[i]);
		}
		RawMesh.WedgeIndices.Append(Indices);

		TArray<FVector2D> TexCoords;
		for (int32 i = 0; i < 36; ++i) {
			TexCoords.Add(TexCoord[i % 6]);
		}
		RawMesh.WedgeTexCoords[0].Append(TexCoords);
	}

	TMap<uint8, uint32> Materials;
	for (int32 i = 0; i < Voxel.Num(); ++i) {
		int32 n = Materials.Num();
		if (!Materials.Find(Voxel[i].I)) {
			Materials.Add(Voxel[i].I, n);
		}
	}
	for (int32 i = 0; i < Voxel.Num(); ++i) {
		int32 n = *Materials.Find(Voxel[i].I);
		for (int32 f = 0; f < 12; ++f) {
			RawMesh.FaceMaterialIndices.Add(n);
		}
	}

	RawMesh.FaceSmoothingMasks.AddZeroed(RawMesh.WedgeIndices.Num() / 3);
	RawMesh.WedgeTangentX.AddZeroed(RawMesh.WedgeIndices.Num());
	RawMesh.WedgeTangentY.AddZeroed(RawMesh.WedgeIndices.Num());
	RawMesh.WedgeTangentZ.AddZeroed(RawMesh.WedgeIndices.Num());
	RawMesh.WedgeColors.AddZeroed(RawMesh.WedgeIndices.Num());

	return true;
}

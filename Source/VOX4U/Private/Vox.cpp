// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "Vox.h"
#include "RawMesh.h"
#include "VoxImportOption.h"

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
FVox::FVox(FArchive& Ar, const UVoxImportOption* ImportOption)
{
	Import(Ar, ImportOption);
}

/**
 * Import vox data from archive
 * @param FArchive& Ar	Read vox data from the archive
 * @return bool	is valid or supported vox data
 */
bool FVox::Import(FArchive& Ar, const UVoxImportOption* ImportOption)
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
			if (ImportOption->bImportXForward) {
				int32 temp = Size.X;
				Size.X = Size.Y;
				Size.Y = temp;
			}
			UE_LOG(LogVox, Verbose, TEXT("SIZE: %s"), *Size.ToString());
		} else if (0 == FCStringAnsi::Strncmp("XYZI", ChunkId, 4)) {
			uint32 NumVoxels;
			Ar << NumVoxels;
			UE_LOG(LogVox, Verbose, TEXT("XYZI: NumVoxels=%d"), NumVoxels);
			FCell Cell;
			for (uint32 i = 0; i < NumVoxels; ++i) {
				Ar << Cell.X << Cell.Y << Cell.Z << Cell.I;
				if (ImportOption->bImportXForward) {
					uint8 temp = Cell.X;
					Cell.X = Size.X - Cell.Y - 1;
					Cell.Y = Size.Y - temp - 1;
				} else {
					Cell.X = Size.X - Cell.X - 1;
				}
				Cell.I = Cell.I - 1;
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
 */
static const FCell Vectors[6] = {
	FCell( 0, 0, 1),	// Up
	FCell( 0, 0,-1),	// Down
	FCell( 1, 0, 0),	// Forward
	FCell(-1, 0, 0),	// Backward
	FCell( 0, 1, 0),	// Right
	FCell( 0,-1, 0)		// Left
};

/**
 *   4---5
 *  /|  /|
 * 7---6 |
 * | 0-|-1
 * |/  |/
 * 3---2
 */
static const FVector Vertexes[8] = {
	FVector(0, 0, 0),
	FVector(1, 0, 0),
	FVector(1, 1, 0),
	FVector(0, 1, 0),
	FVector(0, 0, 1),
	FVector(1, 0, 1),
	FVector(1, 1, 1),
	FVector(0, 1, 1),
};

/**
 *         7 - 4
 *         | U |
 * 4 - 7 - 6 - 5 - 4
 * | B | R | F | L |
 * 0 - 3 - 2 - 1 - 0
 *         | D |
 *         3 - 0
 */
static const uint32 Faces[6][4] = {
	{ 5, 4, 7, 6 },	// Up 		FCell( 0, 0, 1)
	{ 0, 1, 2, 3 },	// Down 	FCell( 0, 0,-1)
	{ 5, 6, 2, 1 },	// Forward 	FCell( 1, 0, 0)
	{ 7, 4, 0, 3 },	// Backward FCell(-1, 0, 0)
	{ 6, 7, 3, 2 },	// Right 	FCell( 0, 1, 0)
	{ 4, 5, 1, 0 }	// Left 	FCell( 0,-1, 0)
};

/**
 * CreateRawMesh
 * @param FRawMesh& RawMesh	Out RawMesh
 * @return Result
 */
bool FVox::CreateRawMesh(FRawMesh& RawMesh, const UVoxImportOption* ImportOption) const
{
	for (FCell Cell : Voxel) {
		FVector Origin(Cell.X, Cell.Y, Cell.Z);
		for (int FaceIndex = 0; FaceIndex < 6; ++FaceIndex) {
			FCell n = Cell + Vectors[FaceIndex];
			int32 CellIndex = Voxel.IndexOfByPredicate([&](const FCell& c) {
				return c.X == n.X && c.Y == n.Y && c.Z == n.Z;
			});
			if (INDEX_NONE != CellIndex) continue;

			TArray<uint32> VertexPositionIndex;
			for (int VertexIndex = 0; VertexIndex < 4; ++VertexIndex) {
				FVector v = Origin + Vertexes[Faces[FaceIndex][VertexIndex]];
				int32 vpi;
				if (!ImportOption->bMergeVertexes
					|| INDEX_NONE == (vpi = RawMesh.VertexPositions.IndexOfByKey(v))) {
					vpi = RawMesh.VertexPositions.Add(v);
				}
				VertexPositionIndex.Add(vpi);
			}

			RawMesh.WedgeIndices.Add(VertexPositionIndex[0]);
			RawMesh.WedgeIndices.Add(VertexPositionIndex[1]);
			RawMesh.WedgeIndices.Add(VertexPositionIndex[2]);
			RawMesh.WedgeTexCoords[0].Add(FVector2D((double)Cell.I / 256.0, 0));
			RawMesh.WedgeTexCoords[0].Add(FVector2D((double)Cell.I / 256.0, 0));
			RawMesh.WedgeTexCoords[0].Add(FVector2D((double)Cell.I / 256.0, 0));
			RawMesh.FaceMaterialIndices.Add(0);
			RawMesh.FaceSmoothingMasks.Add(0);

			RawMesh.WedgeIndices.Add(VertexPositionIndex[2]);
			RawMesh.WedgeIndices.Add(VertexPositionIndex[3]);
			RawMesh.WedgeIndices.Add(VertexPositionIndex[0]);
			RawMesh.WedgeTexCoords[0].Add(FVector2D((double)Cell.I / 256.0, 0));
			RawMesh.WedgeTexCoords[0].Add(FVector2D((double)Cell.I / 256.0, 0));
			RawMesh.WedgeTexCoords[0].Add(FVector2D((double)Cell.I / 256.0, 0));
			RawMesh.FaceMaterialIndices.Add(0);
			RawMesh.FaceSmoothingMasks.Add(0);
		}
	}

	FVector Offset = ImportOption->bImportXYCenter ? FVector((float)Size.X * 0.5f, (float)Size.Y * 0.5f, 0.f) : FVector::ZeroVector;
	for (int32 i = 0; i < RawMesh.VertexPositions.Num(); ++i) {
		FVector VertexPosition = RawMesh.VertexPositions[i];
		RawMesh.VertexPositions[i] = VertexPosition - Offset;
	}

	RawMesh.CompactMaterialIndices();
	check(RawMesh.IsValidOrFixable());

	return true;
}

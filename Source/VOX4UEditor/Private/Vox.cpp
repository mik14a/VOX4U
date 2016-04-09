// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UEditorPrivatePCH.h"
#include "Vox.h"
#include "RawMesh.h"
#include "VoxImportOption.h"

DEFINE_LOG_CATEGORY_STATIC(LogVox, Log, All)

/**
 * MagicaVoxel default palette
 */
static const unsigned int mv_default_palette[256] = {
	0x00000000, 0xffffffff, 0xffccffff, 0xff99ffff, 0xff66ffff, 0xff33ffff, 0xff00ffff, 0xffffccff, 0xffccccff, 0xff99ccff, 0xff66ccff, 0xff33ccff, 0xff00ccff, 0xffff99ff, 0xffcc99ff, 0xff9999ff,
	0xff6699ff, 0xff3399ff, 0xff0099ff, 0xffff66ff, 0xffcc66ff, 0xff9966ff, 0xff6666ff, 0xff3366ff, 0xff0066ff, 0xffff33ff, 0xffcc33ff, 0xff9933ff, 0xff6633ff, 0xff3333ff, 0xff0033ff, 0xffff00ff,
	0xffcc00ff, 0xff9900ff, 0xff6600ff, 0xff3300ff, 0xff0000ff, 0xffffffcc, 0xffccffcc, 0xff99ffcc, 0xff66ffcc, 0xff33ffcc, 0xff00ffcc, 0xffffcccc, 0xffcccccc, 0xff99cccc, 0xff66cccc, 0xff33cccc,
	0xff00cccc, 0xffff99cc, 0xffcc99cc, 0xff9999cc, 0xff6699cc, 0xff3399cc, 0xff0099cc, 0xffff66cc, 0xffcc66cc, 0xff9966cc, 0xff6666cc, 0xff3366cc, 0xff0066cc, 0xffff33cc, 0xffcc33cc, 0xff9933cc,
	0xff6633cc, 0xff3333cc, 0xff0033cc, 0xffff00cc, 0xffcc00cc, 0xff9900cc, 0xff6600cc, 0xff3300cc, 0xff0000cc, 0xffffff99, 0xffccff99, 0xff99ff99, 0xff66ff99, 0xff33ff99, 0xff00ff99, 0xffffcc99,
	0xffcccc99, 0xff99cc99, 0xff66cc99, 0xff33cc99, 0xff00cc99, 0xffff9999, 0xffcc9999, 0xff999999, 0xff669999, 0xff339999, 0xff009999, 0xffff6699, 0xffcc6699, 0xff996699, 0xff666699, 0xff336699,
	0xff006699, 0xffff3399, 0xffcc3399, 0xff993399, 0xff663399, 0xff333399, 0xff003399, 0xffff0099, 0xffcc0099, 0xff990099, 0xff660099, 0xff330099, 0xff000099, 0xffffff66, 0xffccff66, 0xff99ff66,
	0xff66ff66, 0xff33ff66, 0xff00ff66, 0xffffcc66, 0xffcccc66, 0xff99cc66, 0xff66cc66, 0xff33cc66, 0xff00cc66, 0xffff9966, 0xffcc9966, 0xff999966, 0xff669966, 0xff339966, 0xff009966, 0xffff6666,
	0xffcc6666, 0xff996666, 0xff666666, 0xff336666, 0xff006666, 0xffff3366, 0xffcc3366, 0xff993366, 0xff663366, 0xff333366, 0xff003366, 0xffff0066, 0xffcc0066, 0xff990066, 0xff660066, 0xff330066,
	0xff000066, 0xffffff33, 0xffccff33, 0xff99ff33, 0xff66ff33, 0xff33ff33, 0xff00ff33, 0xffffcc33, 0xffcccc33, 0xff99cc33, 0xff66cc33, 0xff33cc33, 0xff00cc33, 0xffff9933, 0xffcc9933, 0xff999933,
	0xff669933, 0xff339933, 0xff009933, 0xffff6633, 0xffcc6633, 0xff996633, 0xff666633, 0xff336633, 0xff006633, 0xffff3333, 0xffcc3333, 0xff993333, 0xff663333, 0xff333333, 0xff003333, 0xffff0033,
	0xffcc0033, 0xff990033, 0xff660033, 0xff330033, 0xff000033, 0xffffff00, 0xffccff00, 0xff99ff00, 0xff66ff00, 0xff33ff00, 0xff00ff00, 0xffffcc00, 0xffcccc00, 0xff99cc00, 0xff66cc00, 0xff33cc00,
	0xff00cc00, 0xffff9900, 0xffcc9900, 0xff999900, 0xff669900, 0xff339900, 0xff009900, 0xffff6600, 0xffcc6600, 0xff996600, 0xff666600, 0xff336600, 0xff006600, 0xffff3300, 0xffcc3300, 0xff993300,
	0xff663300, 0xff333300, 0xff003300, 0xffff0000, 0xffcc0000, 0xff990000, 0xff660000, 0xff330000, 0xff0000ee, 0xff0000dd, 0xff0000bb, 0xff0000aa, 0xff000088, 0xff000077, 0xff000055, 0xff000044,
	0xff000022, 0xff000011, 0xff00ee00, 0xff00dd00, 0xff00bb00, 0xff00aa00, 0xff008800, 0xff007700, 0xff005500, 0xff004400, 0xff002200, 0xff001100, 0xffee0000, 0xffdd0000, 0xffbb0000, 0xffaa0000,
	0xff880000, 0xff770000, 0xff550000, 0xff440000, 0xff220000, 0xff110000, 0xffeeeeee, 0xffdddddd, 0xffbbbbbb, 0xffaaaaaa, 0xff888888, 0xff777777, 0xff555555, 0xff444444, 0xff222222, 0xff111111
};

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

	if (Palette.Num() == 0) {
		for (uint32 i = 0; i < 256; ++i) {
			Palette.Add(FColor(mv_default_palette[i]));
		}
	}
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
static const FIntVector Vectors[6] = {
	FIntVector( 0, 0, 1),	// Up
	FIntVector( 0, 0,-1),	// Down
	FIntVector( 1, 0, 0),	// Forward
	FIntVector(-1, 0, 0),	// Backward
	FIntVector( 0, 1, 0),	// Right
	FIntVector( 0,-1, 0)	// Left
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
			RawMesh.WedgeColors.Add(Palette[Cell.I]);
			RawMesh.WedgeColors.Add(Palette[Cell.I]);
			RawMesh.WedgeColors.Add(Palette[Cell.I]);
			RawMesh.WedgeTexCoords[0].Add(FVector2D((double)Cell.I / 256.0, 0));
			RawMesh.WedgeTexCoords[0].Add(FVector2D((double)Cell.I / 256.0, 0));
			RawMesh.WedgeTexCoords[0].Add(FVector2D((double)Cell.I / 256.0, 0));
			RawMesh.FaceMaterialIndices.Add(0);
			RawMesh.FaceSmoothingMasks.Add(0);

			RawMesh.WedgeIndices.Add(VertexPositionIndex[2]);
			RawMesh.WedgeIndices.Add(VertexPositionIndex[3]);
			RawMesh.WedgeIndices.Add(VertexPositionIndex[0]);
			RawMesh.WedgeColors.Add(Palette[Cell.I]);
			RawMesh.WedgeColors.Add(Palette[Cell.I]);
			RawMesh.WedgeColors.Add(Palette[Cell.I]);
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

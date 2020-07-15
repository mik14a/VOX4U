// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#include "CellMesh.h"
#include "FVoxel.h"
#include "VoxImportOption.h"

CellMesh::CellMesh(const FVoxel* InVox)
	: IMesher(InVox)
{
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
	FIntVector(0, 0, 1),	// Up
	FIntVector(0, 0,-1),	// Down
	FIntVector(1, 0, 0),	// Forward
	FIntVector(-1, 0, 0),	// Backward
	FIntVector(0, 1, 0),	// Right
	FIntVector(0,-1, 0)		// Left
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
 * 2 <- 1
 * | \  ^
 * .  \ |
 * 3 -> 0
 */
static const uint32 Polygons[2][3] = {
	{ 0, 1, 2 },
	{ 2, 3, 0 },
};

/**
 * 2 <- 1
 * | \  ^
 * .  \ |
 * 3 -> 0
 */
static FVector2D TextureCoord[2][3] = {
	{ FVector2D(1.f, 1.f), FVector2D(1.f, 0.f), FVector2D(0.f, 0.f) },
	{ FVector2D(0.f, 0.f), FVector2D(0.f, 1.f), FVector2D(1.f, 1.f) },
};


bool CellMesh::CreateRawMesh(FRawMesh& OutRawMesh, const UVoxImportOption* ImportOption) const
{
	for (const auto& Cell : Vox->Voxel) {
		FVector Origin(Cell.Key.X, Cell.Key.Y, Cell.Key.Z);
		for (int FaceIndex = 0; FaceIndex < 6; ++FaceIndex) {
			const auto n = Cell.Key + Vectors[FaceIndex];
			if (Vox->Voxel.Find(n)) continue;

			TArray<uint32> VertexPositionIndex;
			for (int VertexIndex = 0; VertexIndex < 4; ++VertexIndex) {
				FVector v = Origin + Vertexes[Faces[FaceIndex][VertexIndex]];
				int32 vpi = OutRawMesh.VertexPositions.AddUnique(v);
				VertexPositionIndex.Add(vpi);
			}

			uint8 ColorIndex = Cell.Value - 1;
			for (int PolygonIndex = 0; PolygonIndex < 2; ++PolygonIndex) {
				OutRawMesh.WedgeIndices.Add(VertexPositionIndex[Polygons[PolygonIndex][0]]);
				OutRawMesh.WedgeIndices.Add(VertexPositionIndex[Polygons[PolygonIndex][1]]);
				OutRawMesh.WedgeIndices.Add(VertexPositionIndex[Polygons[PolygonIndex][2]]);
				OutRawMesh.WedgeColors.Add(Vox->Palette[ColorIndex]);
				OutRawMesh.WedgeColors.Add(Vox->Palette[ColorIndex]);
				OutRawMesh.WedgeColors.Add(Vox->Palette[ColorIndex]);
				OutRawMesh.WedgeTexCoords[0].Add(FVector2D(((double)ColorIndex + 0.5) / 256.0, 0.5));
				OutRawMesh.WedgeTexCoords[0].Add(FVector2D(((double)ColorIndex + 0.5) / 256.0, 0.5));
				OutRawMesh.WedgeTexCoords[0].Add(FVector2D(((double)ColorIndex + 0.5) / 256.0, 0.5));
				OutRawMesh.FaceMaterialIndices.Add(0);
				OutRawMesh.FaceSmoothingMasks.Add(0);
			}
		}
	}

	OutRawMesh.CompactMaterialIndices();
	const auto IsValidOrFixable = OutRawMesh.IsValidOrFixable();
	check(IsValidOrFixable);

	return IsValidOrFixable;
}

bool CellMesh::CreateRawMeshes(TArray<FRawMesh>& OutRawMeshes, const UVoxImportOption* ImportOption) const
{
	for (const auto& Cell : Vox->Voxel) {
		FRawMesh OutRawMesh;

		FVector Origin(Cell.Key.X, Cell.Key.Y, Cell.Key.Z);
		for (int VertexIndex = 0; VertexIndex < 8; ++VertexIndex) {
			OutRawMesh.VertexPositions.Add(Origin + Vertexes[VertexIndex]);
		}
		for (int FaceIndex = 0; FaceIndex < 6; ++FaceIndex) {
			uint8 ColorIndex = Cell.Value - 1;
			for (int PolygonIndex = 0; PolygonIndex < 2; ++PolygonIndex) {
				OutRawMesh.WedgeIndices.Add(Faces[FaceIndex][Polygons[PolygonIndex][0]]);
				OutRawMesh.WedgeIndices.Add(Faces[FaceIndex][Polygons[PolygonIndex][1]]);
				OutRawMesh.WedgeIndices.Add(Faces[FaceIndex][Polygons[PolygonIndex][2]]);
				OutRawMesh.WedgeColors.Add(Vox->Palette[ColorIndex]);
				OutRawMesh.WedgeColors.Add(Vox->Palette[ColorIndex]);
				OutRawMesh.WedgeColors.Add(Vox->Palette[ColorIndex]);
				OutRawMesh.WedgeTexCoords[0].Add(FVector2D(((double)ColorIndex + 0.5) / 256.0, 0.5));
				OutRawMesh.WedgeTexCoords[0].Add(FVector2D(((double)ColorIndex + 0.5) / 256.0, 0.5));
				OutRawMesh.WedgeTexCoords[0].Add(FVector2D(((double)ColorIndex + 0.5) / 256.0, 0.5));
				OutRawMesh.FaceMaterialIndices.Add(0);
				OutRawMesh.FaceSmoothingMasks.Add(0);
			}
		}
		OutRawMesh.CompactMaterialIndices();
		OutRawMeshes.Add(OutRawMesh);
	}

	auto ValidOrFixable = true;
	for (const auto& OutRawMesh : OutRawMeshes) {
		const auto _ValidOrFixable = OutRawMesh.IsValidOrFixable();
		ValidOrFixable = ValidOrFixable && _ValidOrFixable;
		check(_ValidOrFixable);
	}
	check(ValidOrFixable);

	return ValidOrFixable;
}

bool CellMesh::CreateBoxCell(FRawMesh& OutRawMesh, const UVoxImportOption* ImportOption)
{
	for (int VertexIndex = 0; VertexIndex < 8; ++VertexIndex) {
		OutRawMesh.VertexPositions.Add(Vertexes[VertexIndex] - FVector(0.5f, 0.5f, 0.5f));
	}
	for (int FaceIndex = 0; FaceIndex < 6; ++FaceIndex) {
		for (int PolygonIndex = 0; PolygonIndex < 2; ++PolygonIndex) {
			OutRawMesh.WedgeIndices.Add(Faces[FaceIndex][Polygons[PolygonIndex][0]]);
			OutRawMesh.WedgeIndices.Add(Faces[FaceIndex][Polygons[PolygonIndex][1]]);
			OutRawMesh.WedgeIndices.Add(Faces[FaceIndex][Polygons[PolygonIndex][2]]);
			OutRawMesh.WedgeTexCoords[0].Add(TextureCoord[PolygonIndex][0]);
			OutRawMesh.WedgeTexCoords[0].Add(TextureCoord[PolygonIndex][1]);
			OutRawMesh.WedgeTexCoords[0].Add(TextureCoord[PolygonIndex][2]);
			OutRawMesh.FaceMaterialIndices.Add(0);
			OutRawMesh.FaceSmoothingMasks.Add(0);
		}
	}
	OutRawMesh.CompactMaterialIndices();
	auto const ValidOrFixable = OutRawMesh.IsValidOrFixable();
	check(ValidOrFixable);
	return ValidOrFixable;
}

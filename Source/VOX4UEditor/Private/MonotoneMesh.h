// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include <RawMesh.h>

struct FFace;
struct FPolygon;
struct FVox;
class UVoxImportOption;

/**
 * Monotone mesh generation
 * @see https://0fps.net/2012/07/07/meshing-minecraft-part-2/
 */
class MonotoneMesh
{
public:

	/** Construct mesh generator */
	MonotoneMesh(const FVox* InVox);

	/** Create FRawMesh from Voxel */
	bool CreateRawMesh(FRawMesh& OutRawMesh, const UVoxImportOption* ImportOption) const;

private:

	void CreatePolygons(TArray<FPolygon>& OutPolygons, const FIntVector& Plane, const FIntVector& Axis) const;
	void CreateFaces(TArray<FFace>& OutFaces, const FIntVector& Plane, const FIntVector& Axis) const;
	void WritePolygon(FRawMesh& OutRawMesh, const FIntVector& Axis, const FPolygon& Polygon) const;

	static void WriteVertex(FRawMesh& OutRawMesh, TArray<int>& OutLeftIndex, TArray<int>& OutRightIndex, const FIntVector& Axis, const FPolygon& Polygon);
	static void WriteWedge(FRawMesh& OutRawMesh, bool Face, int Index1, int Index2, int Index3, int ColorIndex);

private:

	const FVox* Vox;
};

/**
 * @struct FFace
 * Faces in horizontal scan line
 */
struct FFace
{
	/** Voxel color index */
	int Color;
	/** Start left */
	int Left;
	/** End right */
	int Right;

	FFace(int color, int left, int right) {
		Color = color, Left = left, Right = right;
	}
};

/**
 * @struct FPolygon
 * Monotone polygon
 */
struct FPolygon
{
	/** Voxel color index */
	int Color;
	/** Vertices left side */
	TArray<FIntVector> Left;
	/** Vertices right side */
	TArray<FIntVector> Right;

	FPolygon(int color, int left, int right, int y, int z) {
		Color = color;
		Left.Add(FIntVector(left, y, z));
		Right.Add(FIntVector(right, y, z));
	}

	/** Merge cells each scan lines */
	void Merge(int left, int right, int y, int z) {
		auto ll = Left.Last().X;
		auto lr = Right.Last().X;
		if (ll != left) {
			Left.Add(FIntVector(ll, y, z));
			Left.Add(FIntVector(left, y, z));
		}
		if (lr != right) {
			Right.Add(FIntVector(lr, y, z));
			Right.Add(FIntVector(right, y, z));
		}
	}

	/** Close off monotone polygon */
	void CloseOff(int y, int z) {
		auto ll = Left.Last().X;
		auto lr = Right.Last().X;
		Left.Add(FIntVector(ll, y, z));
		Right.Add(FIntVector(lr, y, z));
	}
};

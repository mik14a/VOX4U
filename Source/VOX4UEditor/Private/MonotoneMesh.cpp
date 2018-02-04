// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#include "MonotoneMesh.h"
#include "Vox.h"
#include "VoxImportOption.h"

/**
 * Construct mesh generator using referenced voxel
 */
MonotoneMesh::MonotoneMesh(const FVox* InVox)
{
	Vox = InVox;
}

/**
 * CreateRawMesh
 * Create raw mesh use monotone decomposition algorithm
 */
bool MonotoneMesh::CreateRawMesh(FRawMesh& OutRawMesh, const UVoxImportOption* ImportOption) const
{
	for (auto Dimension = 0; Dimension < 3; ++Dimension) {
		auto Plane = FIntVector::ZeroValue;
		const auto Axis = FIntVector(Dimension, (Dimension + 1) % 3, (Dimension + 2) % 3);
		for (Plane[Axis.Z] = 0; Plane[Axis.Z] <= Vox->Size[Axis.Z]; ++Plane[Axis.Z]) {
			auto Polygons = TArray<FPolygon>();
			CreatePolygons(Polygons, Plane, Axis);
			for (auto i = 0; i < Polygons.Num(); ++i) {
				WritePolygon(OutRawMesh, Axis, Polygons[i]);
			}
		}
	}

	if (ImportOption->bImportXYCenter) {
		FVector Offset = FVector((float)Vox->Size.X * 0.5f, (float)Vox->Size.Y * 0.5f, 0.f);
		for (int32 i = 0; i < OutRawMesh.VertexPositions.Num(); ++i) {
			OutRawMesh.VertexPositions[i] -= Offset;
		}
	}
	OutRawMesh.CompactMaterialIndices();
	return true;
}

/**
 * CreatePolygons
 * Create monotone polygons each voxel types in any faces of volumes
 * @param OutPolygons Out polygons
 * @param Plane Coordinate for polygon faces
 * @param Axis Component index of scan faces
 */
void MonotoneMesh::CreatePolygons(TArray<FPolygon>& OutPolygons, const FIntVector& Plane, const FIntVector& Axis) const
{
	auto P = Plane;
	auto Frontier = TArray<int>();
	for (P[Axis.Y] = 0; P[Axis.Y] < Vox->Size[Axis.Y]; ++P[Axis.Y]) {
		auto Faces = TArray<FFace>();
		CreateFaces(Faces, P, Axis);
		auto NextFrontier = TArray<int>();
		auto FrontierIndex = 0, FaceIndex = 0;
		while (FrontierIndex < Frontier.Num() && FaceIndex < Faces.Num()) {
			auto& Polygon = OutPolygons[Frontier[FrontierIndex]];
			const auto& Color = Polygon.Color;
			const auto& Left = Polygon.Left.Last().X;
			const auto& Right = Polygon.Right.Last().X;
			const auto& Face = Faces[FaceIndex];
			if (Left < Face.Right && Face.Left < Right && Face.Color == Color) {
				Polygon.Merge(Face.Left, Face.Right, P[Axis.Y], P[Axis.Z]);
				NextFrontier.Add(Frontier[FrontierIndex]);
				++FrontierIndex, ++FaceIndex;
			} else {
				if (Right <= Face.Right) {
					Polygon.CloseOff(P[Axis.Y], P[Axis.Z]);
					++FrontierIndex;
				}
				if (Face.Right <= Right) {
					NextFrontier.Add(OutPolygons.Num());
					OutPolygons.Add(FPolygon(Face.Color, Face.Left, Face.Right, P[Axis.Y], P[Axis.Z]));
					++FaceIndex;
				}
			}
		}
		while (FrontierIndex < Frontier.Num()) {
			auto& Polygon = OutPolygons[Frontier[FrontierIndex++]];
			Polygon.CloseOff(P[Axis.Y], P[Axis.Z]);
		}
		while (FaceIndex < Faces.Num()) {
			NextFrontier.Add(OutPolygons.Num());
			const auto& Face = Faces[FaceIndex++];
			OutPolygons.Add(FPolygon(Face.Color, Face.Left, Face.Right, P[Axis.Y], P[Axis.Z]));
		}
		Frontier = NextFrontier;
	}
	for (auto i = 0; i < Frontier.Num(); ++i) {
		auto& Polygon = OutPolygons[Frontier[i]];
		Polygon.CloseOff(P[Axis.Y], P[Axis.Z]);
	}
}

/**
 * CreateFaces
 * Create scan line run faces each side
 * @param OutFaces Out faces
 * @param Plane Coordinate for polygon faces
 * @param Axis Component index of scan faces
 */
void MonotoneMesh::CreateFaces(TArray<FFace>& OutFaces, const FIntVector& Plane, const FIntVector& Axis) const
{
	auto P = Plane;
	auto D = FIntVector();
	D[Axis.X] = 0, D[Axis.Y] = 0, D[Axis.Z] = -1;
	auto PreviouseColor = 0;
	for (P[Axis.X] = 0; P[Axis.X] < Vox->Size[Axis.X]; ++P[Axis.X]) {
		auto Back = Vox->Voxel.FindRef(P + D);
		auto Front = Vox->Voxel.FindRef(P);
		auto Color = !Back == !Front ? 0 : Back ? -Back : Front;
		if (PreviouseColor != Color) {
			if (PreviouseColor != 0) {
				OutFaces.Last().Right = P[Axis.X];
			}
			if (Color != 0) {
				OutFaces.Add(FFace(Color, P[Axis.X], P[Axis.X]));
			}
		}
		PreviouseColor = Color;
	}
	if (PreviouseColor != 0) {
		OutFaces.Last().Right = P[Axis.X];
	}
}

/**
 * WritePolygon
 * Split polygon to triangle mesh
 * @param OutRawMesh Out raw mesh
 * @param Axis Polygon axis
 * @param Polygon Polygon to divide and write
 */
void MonotoneMesh::WritePolygon(FRawMesh& OutRawMesh, const FIntVector& Axis, const FPolygon& Polygon) const
{
	auto LeftIndex = TArray<int>();
	auto RightIndex = TArray<int>();
	WriteVertex(OutRawMesh, LeftIndex, RightIndex, Axis, Polygon);

	const auto Color = 0 < Polygon.Color ? Polygon.Color - 1 : -Polygon.Color - 1;
	const auto Flipped = Polygon.Color < 0;
	auto Left = 1, Right = 1;
	auto LastSide = true;

	const auto CrossProduct = [](const FIntVector& a, const FIntVector& b) -> int {
		return a.X * b.Y - b.X * a.Y;
	};

	auto List = TArray<TPair<int, FIntVector>>();
	List.Add(TPair<int, FIntVector>(LeftIndex[0], Polygon.Left[0]));
	List.Add(TPair<int, FIntVector>(RightIndex[0], Polygon.Right[0]));

	while (Left < Polygon.Left.Num() || Right < Polygon.Right.Num()) {
		auto Side = false;
		if (Left == Polygon.Left.Num()) {
			Side = true;
		} else if (Right != Polygon.Right.Num()) {
			const auto& L = Polygon.Left[Left];
			const auto& R = Polygon.Right[Right];
			Side = L.Y > R.Y;
		}

		auto Index = Side ? RightIndex[Right] : LeftIndex[Left];
		auto Vertex = Side ? Polygon.Right[Right] : Polygon.Left[Left];
		if (Side != LastSide) {
			while (1 < List.Num()) {
				const auto& First = List[0];
				const auto& Second = List[1];
				WriteWedge(OutRawMesh, Flipped == Side, First.Key, Second.Key, Index, Color);
				List.RemoveAt(0);
			}
		} else {
			while (1 < List.Num()) {
				const auto& Last = List[List.Num() - 1];
				const auto& PreviousLast = List[List.Num() - 2];
				const auto Normal = CrossProduct(Last.Value - Vertex, PreviousLast.Value - Vertex);
				if (Side == (Normal > 0)) {
					break;
				}
				if (Normal != 0) {
					WriteWedge(OutRawMesh, Flipped == Side, Last.Key, PreviousLast.Key, Index, Color);
				}
				List.RemoveAt(List.Num() - 1);
			}
		}
		List.Add(TPair<int, FIntVector>(Index, Vertex));
		Side ? ++Right : ++Left;
		LastSide = Side;
	}
}

/**
 * WriteVertex
 * @param OutRawMesh Out raw mesh
 */
void MonotoneMesh::WriteVertex(FRawMesh& OutRawMesh, TArray<int>& OutLeftIndex, TArray<int>& OutRightIndex, const FIntVector& Axis, const FPolygon& Polygon)
{
	for (auto i = 0; i < Polygon.Left.Num(); ++i) {
		auto Vector = Polygon.Left[i];
		auto Vertex = FVector();
		Vertex[Axis.X] = Vector.X;
		Vertex[Axis.Y] = Vector.Y;
		Vertex[Axis.Z] = Vector.Z;
		const auto UniqueIndex = OutRawMesh.VertexPositions.AddUnique(Vertex);
		OutLeftIndex.Add(UniqueIndex);
	}
	for (auto i = 0; i < Polygon.Right.Num(); ++i) {
		auto Vector = Polygon.Right[i];
		auto Vertex = FVector();
		Vertex[Axis.X] = Vector.X;
		Vertex[Axis.Y] = Vector.Y;
		Vertex[Axis.Z] = Vector.Z;
		const auto UniqueIndex = OutRawMesh.VertexPositions.AddUnique(Vertex);
		OutRightIndex.Add(UniqueIndex);
	}
}

/**
 * WriteWedge
 * @param OutRawMesh Out raw mesh
 */
void MonotoneMesh::WriteWedge(FRawMesh& OutRawMesh, bool Face, int Index1, int Index2, int Index3, int ColorIndex)
{
	OutRawMesh.WedgeIndices.Add(Face ? Index1 : Index2);
	OutRawMesh.WedgeIndices.Add(Face ? Index2 : Index1);
	OutRawMesh.WedgeIndices.Add(Index3);
	OutRawMesh.WedgeTexCoords[0].Add(FVector2D(((double)ColorIndex + 0.5) / 256.0, 0.5));
	OutRawMesh.WedgeTexCoords[0].Add(FVector2D(((double)ColorIndex + 0.5) / 256.0, 0.5));
	OutRawMesh.WedgeTexCoords[0].Add(FVector2D(((double)ColorIndex + 0.5) / 256.0, 0.5));
	OutRawMesh.FaceMaterialIndices.Add(0);
	OutRawMesh.FaceSmoothingMasks.Add(0);
}

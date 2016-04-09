// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "InstancedVoxel.h"

UInstancedVoxel::UInstancedVoxel()
	: Size(ForceInit)
	, CellBounds(FVector::ZeroVector, FVector(100.f, 100.f, 100.f), 100.f)
	, bXYCenter(true)
	, Mesh()
	, Voxel()
{
}

#if WITH_EDITOR

void UInstancedVoxel::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	static const FName NAME_Mesh = FName(TEXT("Mesh"));
	if (PropertyChangedEvent.Property) {
		if (PropertyChangedEvent.Property->GetFName() == NAME_Mesh) {
			CalcCellBounds();
		}
	}
}

void UInstancedVoxel::CalcCellBounds()
{
	FBoxSphereBounds Bounds(ForceInit);
	for (const auto* Mesh : this->Mesh.FilterByPredicate([](UStaticMesh* m) { return !!m; })) {
		Bounds = Bounds + Mesh->GetBounds();
	}
	CellBounds = Bounds;
}

#endif // WITH_EDITOR

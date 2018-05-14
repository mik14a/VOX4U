// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#include "Voxel.h"

UVoxel::UVoxel()
	: Size(ForceInit)
	, CellBounds(FVector::ZeroVector, FVector(100.f, 100.f, 100.f), 100.f)
	, bXYCenter(true)
	, Mesh()
	, Voxel()
{
}

void UVoxel::PostInitProperties()
{
#if WITH_EDITORONLY_DATA
	if (!HasAnyFlags(RF_ClassDefaultObject) && !(GetOuter() && GetOuter()->HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))) {
		AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
	}
#endif
	Super::PostInitProperties();
}

#if WITH_EDITOR

void UVoxel::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	static const FName NAME_Mesh = FName(TEXT("Mesh"));
	if (PropertyChangedEvent.Property) {
		if (PropertyChangedEvent.Property->GetFName() == NAME_Mesh) {
			CalcCellBounds();
		}
	}
}

void UVoxel::CalcCellBounds()
{
	FBoxSphereBounds Bounds(ForceInit);
	for (const auto* Mesh : this->Mesh.FilterByPredicate([](UStaticMesh* m) { return !!m; })) {
		Bounds = Bounds + Mesh->GetBounds();
	}
	CellBounds = Bounds;
}

#endif // WITH_EDITOR

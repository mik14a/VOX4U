// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#include "FVoxel.h"
#include <Engine/Texture2D.h>
#include "VoxFormat.h"
#include "Importer/VoxExtensionImporter.h"
#include "Importer/VoxImporter.h"
#include "Mesher/CellMesh.h"
#include "Mesher/IMesher.h"
#include "Mesher/MonotoneMesh.h"
#include "VoxImportOption.h"
#include "Voxel.h"

DEFINE_LOG_CATEGORY_STATIC(LogVox, Log, All)

/**
 * Create empty vox data.
 */
FVoxel::FVoxel() : Min(ForceInit), Max(ForceInit)
{
}

/**
 * Create vox data from archive
 * @param Filename Filename
 * @param Data Buffer to use as the source data to read from
 * @param Size Size of Data
 * @param ImportOption
 */
FVoxel::FVoxel(const FString& Filename, const void* Data, int64 Size, const UVoxImportOption* ImportOption)
	: Min(ForceInit), Max(ForceInit)
{
	this->Filename = Filename;
	this->ImportOption = ImportOption;
	FVox vox = ReadVox(Data, Size);

	auto ExtensionFormat = !vox.Node.empty() || !vox.Layer.empty();
	auto Importer = TUniquePtr<IVoxImporter>(
		ExtensionFormat ? static_cast<IVoxImporter*>(new VoxExtensionImporter(this)) : static_cast<IVoxImporter*>(new VoxImporter(this))
	);
	Importer->Import(vox);

	// Centering
	if (ImportOption->bImportXYCenter) {
		const auto Volume = Max - Min + FIntVector(1, 1, 1);
		const auto Offset = FIntVector(Min.X + Volume.X / 2, Min.Y + Volume.Y / 2, 0);
		auto Temp = TMap<FIntVector, uint8>();
		Temp.Reserve(Voxel.Num());
		for (const auto& Cell : Voxel) {
			auto Vector = Cell.Key - Offset;
			Temp.Emplace(MoveTemp(Vector), Cell.Value);
		}
		Voxel = MoveTemp(Temp);
		Min = Max = FIntVector::ZeroValue;
		for (const auto& Cell : Voxel) {
			Min.X = FMath::Min(Min.X, Cell.Key.X);
			Max.X = FMath::Max(Max.X, Cell.Key.X);
			Min.Y = FMath::Min(Min.Y, Cell.Key.Y);
			Max.Y = FMath::Max(Max.Y, Cell.Key.Y);
			Min.Z = FMath::Min(Min.Z, Cell.Key.Z);
			Max.Z = FMath::Max(Max.Z, Cell.Key.Z);
		}
	}

	// X forwarding
	if (ImportOption->bImportXForward) {
		auto Temp = TMap<FIntVector, uint8>();
		Temp.Reserve(Voxel.Num());
		for (const auto& Cell : Voxel) {
			auto Vector = FIntVector(-1 - Cell.Key.Y, -1 - Cell.Key.X, Cell.Key.Z);
			Temp.Emplace(MoveTemp(Vector), Cell.Value);
		}
		Voxel = MoveTemp(Temp);
		Min = Max = FIntVector::ZeroValue;
		for (const auto& Cell : Voxel) {
			Min.X = FMath::Min(Min.X, Cell.Key.X);
			Max.X = FMath::Max(Max.X, Cell.Key.X);
			Min.Y = FMath::Min(Min.Y, Cell.Key.Y);
			Max.Y = FMath::Max(Max.Y, Cell.Key.Y);
			Min.Z = FMath::Min(Min.Z, Cell.Key.Z);
			Max.Z = FMath::Max(Max.Z, Cell.Key.Z);
		}
	}
}

FVoxel::~FVoxel()
{
}


/**
 * CreateRawMesh
 * @param FRawMesh& RawMesh	Out RawMesh
 * @return Result
 */
bool FVoxel::CreateRawMesh(FRawMesh& OutRawMesh) const
{
	auto Mesher = TUniquePtr<IMesher>(
		ImportOption->bImportMeshOptimize ? static_cast<IMesher*>(new MonotoneMesh(this)) : static_cast<IMesher*>(new CellMesh(this))
	);
	return Mesher->CreateRawMesh(OutRawMesh, ImportOption);
}

/**
 * CreateRawMesh
 * @param FRawMesh& RawMesh	Out RawMesh
 * @return Result
 */
bool FVoxel::CreateRawMeshes(TArray<FRawMesh>& OutRawMeshes) const
{
	CellMesh Mesher(this);
	return Mesher.CreateRawMeshes(OutRawMeshes, ImportOption);
}

bool FVoxel::CreateTexture(UTexture2D* const& OutTexture) const
{
	check(OutTexture);
	OutTexture->LODGroup = TextureGroup::TEXTUREGROUP_World;
	OutTexture->CompressionSettings = TextureCompressionSettings::TC_Default;
	OutTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	OutTexture->DeferCompression = true;
	OutTexture->Source.Init(256, 1, 1, 1, TSF_BGRA8, (const uint8*)Palette.GetData());
	OutTexture->UpdateResource();
	OutTexture->PostEditChange();
	return true;
}

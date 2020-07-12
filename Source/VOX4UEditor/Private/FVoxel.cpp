// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#include "FVoxel.h"
#include <Engine/Texture2D.h>
#include "Importer/VoxExtensionImporter.h"
#include "Importer/VoxImporter.h"
#include "Mesher/CellMesh.h"
#include "Mesher/MonotoneMesh.h"
#include "VoxImportOption.h"
#include "vox.h"

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
	auto vox = vox::read(Data, Size);

	auto ExtensionFormat = !vox.node.empty() || !vox.layer.empty();
	auto Importer = TUniquePtr<IVoxImporter>(
		ExtensionFormat ? static_cast<IVoxImporter*>(new VoxExtensionImporter(this)) : static_cast<IVoxImporter*>(new VoxImporter(this))
	);
	Importer->Import(vox);

	auto Temp = Voxel;
	Voxel.Reset();
	for (const auto& Cell : Temp) {
		auto vector = ImportOption->bImportXForward
			? FIntVector(-1 - Cell.Key.Y, -1 - Cell.Key.X, Cell.Key.Z)
			: FIntVector(Cell.Key.X, Cell.Key.Y, Cell.Key.Z);
		Voxel.Add(MoveTemp(vector), Cell.Value);
	}
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
	CellMesh Mesher(this);
	return Mesher.CreateRawMesh(OutRawMesh, ImportOption);
}

/**
 * CreateOptimizedRawMesh
 * @param OutRawMesh Out raw mesh
 * @return Result
 */
bool FVoxel::CreateOptimizedRawMesh(FRawMesh& OutRawMesh) const
{
	MonotoneMesh Mesher(this);
	return Mesher.CreateRawMesh(OutRawMesh, ImportOption);
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

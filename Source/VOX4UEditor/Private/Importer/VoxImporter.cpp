// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#include "VoxImporter.h"
#include "VoxFormat.h"
#include "FVoxel.h"

VoxImporter::VoxImporter(FVoxel* voxel)
	: IVoxImporter(voxel)
{}

void VoxImporter::Import(const FVox& vox)
{
	const auto& size = vox.Size[0];
	Voxel->Min.X = Voxel->Min.Y = Voxel->Min.Z = 0;
	Voxel->Max.X = size.X - 1;
	Voxel->Max.Y = size.Y - 1;
	Voxel->Max.Z = size.Z - 1;

	const auto& voxel = vox.Voxel[0];
	for (const auto& cell : voxel.Voxels) {
		auto vector = FIntVector(cell.x, cell.y, cell.z);
		Voxel->Voxel.Add(MoveTemp(vector), cell.i);
	}
	const auto& palette = vox.Palette.Palettes;
	if (!palette.empty()) {
		for (const auto& color : palette) {
			Voxel->Palette.Add(FColor(color.r, color.g, color.b, color.a));
		}
	} else {
		for (const auto& color : FVox::DefaultPalette) {
			Voxel->Palette.Add(FColor(color));
		}
	}
}

// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#include "VoxImporter.h"
#include "FVoxel.h"
#include "libvox.h"

VoxImporter::VoxImporter(FVoxel* voxel)
	: IVoxImporter(voxel)
{}

void VoxImporter::Import(const vox::vox& vox)
{
	const auto& size = vox.size[0];
	Voxel->Min.X = Voxel->Min.Y = Voxel->Min.Z = 0;
	Voxel->Max.X = size.x - 1;
	Voxel->Max.Y = size.y - 1;
	Voxel->Max.Z = size.z - 1;

	const auto& voxel = vox.voxel[0];
	for (const auto& cell : voxel.voxels) {
		auto vector = FIntVector(cell.x, cell.y, cell.z);
		Voxel->Voxel.Add(MoveTemp(vector), cell.i);
	}
	const auto& palette = vox.palette.palettes;
	if (!palette.empty()) {
		for (const auto& color : palette) {
			Voxel->Palette.Add(FColor(color.r, color.g, color.b, color.a));
		}
	} else {
		for (const auto& color : vox::vox::default_palette) {
			Voxel->Palette.Add(FColor(color));
		}
	}
}

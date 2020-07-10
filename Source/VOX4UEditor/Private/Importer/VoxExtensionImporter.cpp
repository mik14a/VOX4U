// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#include "VoxExtensionImporter.h"
#include "FVoxel.h"
#include "libvox.h"

DEFINE_LOG_CATEGORY_STATIC(LogVoxImporter, Log, All)

const VoxExtensionImporter::TypeIndexMapT<VoxExtensionImporter::SceneGraphOperatorT> VoxExtensionImporter::SceneGraphOperator =
{
	{ vox::transform::tag, &VoxExtensionImporter::Transform },
	{ vox::group::tag, &VoxExtensionImporter::Group},
	{ vox::shape::tag, &VoxExtensionImporter::Shape},
};

VoxExtensionImporter::VoxExtensionImporter(FVoxel* voxel)
	: IVoxImporter(voxel)
	, Depth(0)
{}

void VoxExtensionImporter::Import(const vox::vox& vox)
{
	auto root = vox.node.find(0);
	auto translation = vox::translation();
	auto rotation = vox::rotation();
	if (root != vox.node.end()) {
		auto node = root->second;
		Visit(vox, node, translation, rotation);
	}
	for (const auto& Cell : Voxel->Voxel) {
		Voxel->Min.X = FMath::Min(Voxel->Min.X, Cell.Key.X);
		Voxel->Max.X = FMath::Max(Voxel->Max.X, Cell.Key.X);
		Voxel->Min.Y = FMath::Min(Voxel->Min.Y, Cell.Key.Y);
		Voxel->Max.Y = FMath::Max(Voxel->Max.Y, Cell.Key.Y);
		Voxel->Min.Z = FMath::Min(Voxel->Min.Z, Cell.Key.Z);
		Voxel->Max.Z = FMath::Max(Voxel->Max.Z, Cell.Key.Z);
	}
	const auto& palette = vox.palette.palettes;
	for (const auto& color : palette) {
		Voxel->Palette.Add(FColor(color.r, color.g, color.b, color.a));
	}
}

#define TAG(tag)	(tag) & 0xff, ((tag) >> 8) & 0xff, ((tag) >> 16) & 0xff, ((tag) >> 24) & 0xff

void VoxExtensionImporter::Visit(const vox::vox& vox, std::shared_ptr<vox::node> node, const vox::translation& translation, const vox::rotation& rotation)
{
	auto function = SceneGraphOperator.Find(node->tag);
	if (function) {
		++Depth, (this->* * function)(vox, node, translation, rotation);
	} else {
		UE_LOG(LogVoxImporter, Warning, TEXT("Tag: %c%c%c%c"), TAG(node->tag));
	}
}

#define UE_LOG_TRANSFORM(CategoryName, Verbosity, Depth, Transform)	\
UE_LOG(CategoryName, Verbosity, TEXT("%s* Transfrom: Name[%s], Translation[%d,%d,%d], Rotation[[%d,%d,%d][%d,%d,%d][%d,%d,%d]]"),	\
	* FString::ChrN(Depth, ' '),	\
	* FString(Transform->name.c_str()),	\
	Transform->frame[0].translation.x, Transform->frame[0].translation.y, Transform->frame[0].translation.z,	\
	Transform->frame[0].rotation.m[0][0], Transform->frame[0].rotation.m[0][1], Transform->frame[0].rotation.m[0][2],	\
	Transform->frame[0].rotation.m[1][0], Transform->frame[0].rotation.m[1][1], Transform->frame[0].rotation.m[1][2],	\
	Transform->frame[0].rotation.m[2][0], Transform->frame[0].rotation.m[2][1], Transform->frame[0].rotation.m[2][2]	\
)

void VoxExtensionImporter::Transform(const vox::vox& vox, std::shared_ptr<vox::node> node, const vox::translation& translation, const vox::rotation& rotation)
{
	const auto transform = std::static_pointer_cast<vox::transform>(node);
	UE_LOG_TRANSFORM(LogVoxImporter, Log, Depth, transform);

	const auto& tname = transform->name;
	const auto& ftranslation = transform->frame[0].translation;
	const auto& frotation = transform->frame[0].rotation;
	Visit(vox, vox.node.at(transform->child), ftranslation, frotation);
}

#define UE_LOG_GROUP(CategoryName, Verbosity, Depth, Group)	\
UE_LOG(CategoryName, Verbosity, TEXT("%s* Group"), *FString::ChrN(Depth, ' '))

void VoxExtensionImporter::Group(const vox::vox& vox, std::shared_ptr<vox::node> node, const vox::translation& translation, const vox::rotation& rotation)
{
	const auto group = std::static_pointer_cast<vox::group>(node);
	UE_LOG_GROUP(LogVoxImporter, Log, Depth, group);

	for (const auto& child : group->child) {
		Visit(vox, vox.node.at(child), translation, rotation);
	}
}

#define UE_LOG_SHAPE(CategoryName, Verbosity, Depth, Voxel, Shape)	\
UE_LOG(CategoryName, Verbosity, TEXT("%s* Shape: Id[%d], Size[%d,%d,%d]"),	\
	* FString::ChrN(Depth, ' '),	\
	Shape->model[0].id,	\
	Voxel.size[Shape->model[0].id].x, Voxel.size[Shape->model[0].id].y, Voxel.size[Shape->model[0].id].z	\
)

void VoxExtensionImporter::Shape(const vox::vox& vox, std::shared_ptr<vox::node> node, const vox::translation& translation, const vox::rotation& rotation)
{
	const auto shape = std::static_pointer_cast<vox::shape>(node);
	UE_LOG_SHAPE(LogVoxImporter, Log, Depth, vox, shape);

	const auto sid = shape->model[0].id;
	const auto& ssize = vox.size[sid];
	const auto center = Transform(rotation, vox::translation{ ssize.x / 2, ssize.y / 2, ssize.z / 2 });
	const auto Translation = FIntVector{ translation.x - center.x, translation.y - center.y, translation.z - center.z };
	const auto Delta = Transform(rotation, FIntVector{ 1, 1, 1 });
	const auto Correct = FIntVector(Delta.X < 0 ? -1 : 0, Delta.Y < 0 ? -1 : 0, Delta.Z < 0 ? -1 : 0);
	const auto& svoxel = vox.voxel[sid];
	for (const auto& cell : svoxel.voxels) {
		auto Cell = Transform(rotation, FIntVector{ cell.x, cell.y, cell.z });
		auto Vector = Translation + Cell + Correct;
		Voxel->Voxel.Emplace(MoveTemp(Vector), cell.i);
	}
}

vox::translation VoxExtensionImporter::Transform(const vox::rotation& M, const vox::translation& T)
{
	return {
		T.x * M.m[0][0] + T.y * M.m[1][0] + T.z * M.m[2][0],
		T.x * M.m[0][1] + T.y * M.m[1][1] + T.z * M.m[2][1],
		T.x * M.m[0][2] + T.y * M.m[1][2] + T.z * M.m[2][2]
	};
}

FIntVector VoxExtensionImporter::Transform(const vox::rotation& M, const FIntVector& V)
{
	return {
		V.X * M.m[0][0] + V.Y * M.m[1][0] + V.Z * M.m[2][0],
		V.X * M.m[0][1] + V.Y * M.m[1][1] + V.Z * M.m[2][1],
		V.X * M.m[0][2] + V.Y * M.m[1][2] + V.Z * M.m[2][2]
	};
}

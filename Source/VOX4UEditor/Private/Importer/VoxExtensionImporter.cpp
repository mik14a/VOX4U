// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#include "VoxExtensionImporter.h"
#include "VoxFormat.h"
#include "FVoxel.h"

DEFINE_LOG_CATEGORY_STATIC(LogVoxImporter, Log, All)

const VoxExtensionImporter::TypeIndexMapT<VoxExtensionImporter::SceneGraphOperatorT> VoxExtensionImporter::SceneGraphOperator =
{
	{ FVoxNodeTransform::Tag, &VoxExtensionImporter::Transform },
	{ FVoxNodeGroup::Tag, &VoxExtensionImporter::Group},
	{ FVoxNodeShape::Tag, &VoxExtensionImporter::Shape},
};

VoxExtensionImporter::VoxExtensionImporter(FVoxel* voxel)
	: IVoxImporter(voxel)
	, Depth(0)
{}

void VoxExtensionImporter::Import(const FVox& vox)
{
	auto root = vox.Node.find(0);
	auto translation = ::translation();
	auto rotation = ::rotation();
	if (root != vox.Node.end()) {
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
	const auto& palette = vox.Palette.Palettes;
	for (const auto& color : palette) {
		Voxel->Palette.Add(FColor(color.r, color.g, color.b, color.a));
	}
}

#define TAG(tag)	(tag) & 0xff, ((tag) >> 8) & 0xff, ((tag) >> 16) & 0xff, ((tag) >> 24) & 0xff

void VoxExtensionImporter::Visit(const FVox& vox, std::shared_ptr<FVoxNode> node, const translation& translation, const rotation& rotation)
{
	auto function = SceneGraphOperator.Find(node->Tag);
	if (function) {
		++Depth, (this->* * function)(vox, node, translation, rotation);
	} else {
		UE_LOG(LogVoxImporter, Warning, TEXT("Tag: %c%c%c%c"), TAG(node->Tag));
	}
}

#define UE_LOG_TRANSFORM(CategoryName, Verbosity, Depth, Transform)	\
UE_LOG(CategoryName, Verbosity, TEXT("%s* Transfrom: Name[%s], Translation[%d,%d,%d], Rotation[[%d,%d,%d][%d,%d,%d][%d,%d,%d]]"),	\
	* FString::ChrN(Depth, ' '),	\
	* FString(Transform->Name.c_str()),	\
	Transform->Frame[0].translation.x, Transform->Frame[0].translation.y, Transform->Frame[0].translation.z,	\
	Transform->Frame[0].rotation.m[0][0], Transform->Frame[0].rotation.m[0][1], Transform->Frame[0].rotation.m[0][2],	\
	Transform->Frame[0].rotation.m[1][0], Transform->Frame[0].rotation.m[1][1], Transform->Frame[0].rotation.m[1][2],	\
	Transform->Frame[0].rotation.m[2][0], Transform->Frame[0].rotation.m[2][1], Transform->Frame[0].rotation.m[2][2]	\
)

void VoxExtensionImporter::Transform(const FVox& vox, std::shared_ptr<FVoxNode> node, const ::translation& translation, const ::rotation& rotation)
{
	const auto transform = std::static_pointer_cast<FVoxNodeTransform>(node);
	UE_LOG_TRANSFORM(LogVoxImporter, Log, Depth, transform);

	const auto& tname = transform->Name;
	const auto& ftranslation = transform->Frame[0].translation;
	const auto& frotation = transform->Frame[0].rotation;
	Visit(vox, vox.Node.at(transform->Child), ftranslation, frotation);
}

#define UE_LOG_GROUP(CategoryName, Verbosity, Depth, Group)	\
UE_LOG(CategoryName, Verbosity, TEXT("%s* Group"), *FString::ChrN(Depth, ' '))

void VoxExtensionImporter::Group(const FVox& vox, std::shared_ptr<FVoxNode> node, const ::translation& translation, const ::rotation& rotation)
{
	const auto group = std::static_pointer_cast<FVoxNodeGroup>(node);
	UE_LOG_GROUP(LogVoxImporter, Log, Depth, group);

	for (const auto& child : group->Child) {
		Visit(vox, vox.Node.at(child), translation, rotation);
	}
}

#define UE_LOG_SHAPE(CategoryName, Verbosity, Depth, Voxel, Shape)	\
UE_LOG(CategoryName, Verbosity, TEXT("%s* Shape: Id[%d], Size[%d,%d,%d]"),	\
	* FString::ChrN(Depth, ' '),	\
	Shape->Model[0].Id,	\
	Voxel.Size[Shape->Model[0].Id].Z, Voxel.Size[Shape->Model[0].Id].Y, Voxel.Size[Shape->Model[0].Id].Z	\
)

void VoxExtensionImporter::Shape(const FVox& vox, std::shared_ptr<FVoxNode> node, const ::translation& translation, const ::rotation& rotation)
{
	const auto shape = std::static_pointer_cast<FVoxNodeShape>(node);
	UE_LOG_SHAPE(LogVoxImporter, Log, Depth, vox, shape);

	const auto sid = shape->Model[0].Id;
	const auto& ssize = vox.Size[sid];
	const auto center = Transform(rotation, ::translation{ ssize.X / 2, ssize.Y / 2, ssize.Z / 2 });
	const auto Translation = FIntVector{ translation.x - center.x, translation.y - center.y, translation.z - center.z };
	const auto Delta = Transform(rotation, FIntVector{ 1, 1, 1 });
	const auto Correct = FIntVector(Delta.X < 0 ? -1 : 0, Delta.Y < 0 ? -1 : 0, Delta.Z < 0 ? -1 : 0);
	const auto& svoxel = vox.Voxel[sid];
	for (const auto& cell : svoxel.Voxels) {
		auto Cell = Transform(rotation, FIntVector{ cell.x, cell.y, cell.z });
		auto Vector = Translation + Cell + Correct;
		Voxel->Voxel.Emplace(MoveTemp(Vector), cell.i);
	}
}

::translation VoxExtensionImporter::Transform(const ::rotation& M, const ::translation& T)
{
	return {
		T.x * M.m[0][0] + T.y * M.m[1][0] + T.z * M.m[2][0],
		T.x * M.m[0][1] + T.y * M.m[1][1] + T.z * M.m[2][1],
		T.x * M.m[0][2] + T.y * M.m[1][2] + T.z * M.m[2][2]
	};
}

FIntVector VoxExtensionImporter::Transform(const ::rotation& M, const FIntVector& V)
{
	return {
		V.X * M.m[0][0] + V.Y * M.m[1][0] + V.Z * M.m[2][0],
		V.X * M.m[0][1] + V.Y * M.m[1][1] + V.Z * M.m[2][1],
		V.X * M.m[0][2] + V.Y * M.m[1][2] + V.Z * M.m[2][2]
	};
}

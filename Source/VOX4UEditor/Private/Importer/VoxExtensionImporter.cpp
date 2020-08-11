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
	auto root = vox.Node.Find(0);
	auto translation = FVoxTranslation();
	auto rotation = FVoxRotation();
	if (root != nullptr) {
		auto node = *root;
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
		Voxel->Palette.Add(FColor(color.R, color.G, color.B, color.A));
	}
}

#define TAG(tag)	(tag) & 0xff, ((tag) >> 8) & 0xff, ((tag) >> 16) & 0xff, ((tag) >> 24) & 0xff

void VoxExtensionImporter::Visit(const FVox& vox, TSharedPtr<FVoxNode> node, const FVoxTranslation& translation, const FVoxRotation& rotation)
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
	*FString::ChrN(Depth, ' '),	\
	*Transform->Name,	\
	Transform->Frame[0].Translation.X, Transform->Frame[0].Translation.Y, Transform->Frame[0].Translation.Z,	\
	Transform->Frame[0].Rotation.M[0][0], Transform->Frame[0].Rotation.M[0][1], Transform->Frame[0].Rotation.M[0][2],	\
	Transform->Frame[0].Rotation.M[1][0], Transform->Frame[0].Rotation.M[1][1], Transform->Frame[0].Rotation.M[1][2],	\
	Transform->Frame[0].Rotation.M[2][0], Transform->Frame[0].Rotation.M[2][1], Transform->Frame[0].Rotation.M[2][2]	\
)

void VoxExtensionImporter::Transform(const FVox& vox, TSharedPtr<FVoxNode> node, const FVoxTranslation& translation, const FVoxRotation& rotation)
{
	const auto transform = StaticCastSharedPtr<FVoxNodeTransform>(node);
	UE_LOG_TRANSFORM(LogVoxImporter, Log, Depth, transform);

	const auto& tname = transform->Name;
	const auto& ftranslation = transform->Frame[0].Translation;
	const auto& frotation = transform->Frame[0].Rotation;
	Visit(vox, *vox.Node.Find(transform->Child), ftranslation, frotation);
}

#define UE_LOG_GROUP(CategoryName, Verbosity, Depth, Group)	\
UE_LOG(CategoryName, Verbosity, TEXT("%s* Group"), *FString::ChrN(Depth, ' '))

void VoxExtensionImporter::Group(const FVox& vox, TSharedPtr<FVoxNode> node, const FVoxTranslation& translation, const FVoxRotation& rotation)
{
	const auto group = StaticCastSharedPtr<FVoxNodeGroup>(node);
	UE_LOG_GROUP(LogVoxImporter, Log, Depth, group);

	for (const auto& child : group->Child) {
		Visit(vox, *vox.Node.Find(child), translation, rotation);
	}
}

#define UE_LOG_SHAPE(CategoryName, Verbosity, Depth, Voxel, Shape)	\
UE_LOG(CategoryName, Verbosity, TEXT("%s* Shape: Id[%d], Size[%d,%d,%d]"),	\
	* FString::ChrN(Depth, ' '),	\
	Shape->Model[0].Id,	\
	Voxel.Size[Shape->Model[0].Id].Z, Voxel.Size[Shape->Model[0].Id].Y, Voxel.Size[Shape->Model[0].Id].Z	\
)

void VoxExtensionImporter::Shape(const FVox& vox, TSharedPtr<FVoxNode> node, const FVoxTranslation& translation, const FVoxRotation& rotation)
{
	const auto shape = StaticCastSharedPtr<FVoxNodeShape>(node);
	UE_LOG_SHAPE(LogVoxImporter, Log, Depth, vox, shape);

	const auto sid = shape->Model[0].Id;
	const auto& ssize = vox.Size[sid];
	const auto center = Transform(rotation, FVoxTranslation{ ssize.X / 2, ssize.Y / 2, ssize.Z / 2 });
	const auto Translation = FIntVector{ translation.X - center.X, translation.Y - center.Y, translation.Z - center.Z };
	const auto Delta = Transform(rotation, FIntVector{ 1, 1, 1 });
	const auto Correct = FIntVector(Delta.X < 0 ? -1 : 0, Delta.Y < 0 ? -1 : 0, Delta.Z < 0 ? -1 : 0);
	const auto& svoxel = vox.Voxel[sid];
	for (const auto& cell : svoxel.Voxels) {
		auto Cell = Transform(rotation, FIntVector{ cell.x, cell.y, cell.z });
		auto Vector = Translation + Cell + Correct;
		Voxel->Voxel.Emplace(MoveTemp(Vector), cell.i);
	}
}

FVoxTranslation VoxExtensionImporter::Transform(const FVoxRotation& M, const FVoxTranslation& T)
{
	return {
		T.X * M.M[0][0] + T.Y * M.M[1][0] + T.Z * M.M[2][0],
		T.X * M.M[0][1] + T.Y * M.M[1][1] + T.Z * M.M[2][1],
		T.X * M.M[0][2] + T.Y * M.M[1][2] + T.Z * M.M[2][2]
	};
}

FIntVector VoxExtensionImporter::Transform(const FVoxRotation& M, const FIntVector& V)
{
	return {
		V.X * M.M[0][0] + V.Y * M.M[1][0] + V.Z * M.M[2][0],
		V.X * M.M[0][1] + V.Y * M.M[1][1] + V.Z * M.M[2][1],
		V.X * M.M[0][2] + V.Y * M.M[1][2] + V.Z * M.M[2][2]
	};
}

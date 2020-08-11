// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IVoxImporter.h"

/**
 * VoxImporter
 */
struct VoxExtensionImporter : public IVoxImporter
{
	using SceneGraphOperatorT = void(VoxExtensionImporter::*)(const FVox&, TSharedPtr<FVoxNode>, const FVoxTranslation&, const FVoxRotation&);
	template <typename T> using TypeIndexMapT = TMap<uint32, T>;
	static const TypeIndexMapT<SceneGraphOperatorT> SceneGraphOperator;

public:
	/** Construct importer use parent voxel reference */
	VoxExtensionImporter(FVoxel* voxel);

	/** Import voxel model from vox object */
	void Import(const FVox& vox) override;

protected:
	/** Visit scene graph */
	void Visit(const FVox& vox, TSharedPtr<FVoxNode> node, const FVoxTranslation& translation, const FVoxRotation& rotation);
	/** Visit transform node (nTRN) */
	void Transform(const FVox& vox, TSharedPtr<FVoxNode> node, const FVoxTranslation& translation, const FVoxRotation& rotation);
	/** Visit group node (nGRP) */
	void Group(const FVox& vox, TSharedPtr<FVoxNode> node, const FVoxTranslation& translation, const FVoxRotation& rotation);
	/** Visit shape node (nSHP) */
	void Shape(const FVox& vox, TSharedPtr<FVoxNode> node, const FVoxTranslation& translation, const FVoxRotation& rotation);

private:
	/** Transform translation */
	static FVoxTranslation Transform(const FVoxRotation& M, const FVoxTranslation& T);
	/** Transform vector */
	static FIntVector Transform(const FVoxRotation& M, const FIntVector& V);

private: // Immutable members
	/** Scene graph depth */
	int Depth;
};

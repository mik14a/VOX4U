// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IVoxImporter.h"

/**
 * VoxImporter
 */
struct VoxExtensionImporter : public IVoxImporter
{
	using SceneGraphOperatorT = void(VoxExtensionImporter::*)(const FVox&, std::shared_ptr<FVoxNode>, const ::translation&, const ::rotation&);
	template <typename T> using TypeIndexMapT = TMap<uint32_t, T>;
	static const TypeIndexMapT<SceneGraphOperatorT> SceneGraphOperator;

public:
	/** Construct importer use parent voxel reference */
	VoxExtensionImporter(FVoxel* voxel);

	/** Import voxel model from vox object */
	void Import(const FVox& vox) override;

protected:
	/** Visit scene graph */
	void Visit(const FVox& vox, std::shared_ptr<FVoxNode> node, const ::translation& translation, const ::rotation& rotation);
	/** Visit transform node (nTRN) */
	void Transform(const FVox& vox, std::shared_ptr<FVoxNode> node, const ::translation& translation, const ::rotation& rotation);
	/** Visit group node (nGRP) */
	void Group(const FVox& vox, std::shared_ptr<FVoxNode> node, const ::translation& translation, const ::rotation& rotation);
	/** Visit shape node (nSHP) */
	void Shape(const FVox& vox, std::shared_ptr<FVoxNode> node, const ::translation& translation, const ::rotation& rotation);

private:
	/** Transform translation */
	static ::translation Transform(const ::rotation& M, const ::translation& T);
	/** Transform vector */
	static FIntVector Transform(const ::rotation& M, const FIntVector& V);

private: // Immutable members
	/** Scene graph depth */
	int Depth;
};

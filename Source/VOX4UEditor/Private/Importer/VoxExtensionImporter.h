// Copyright 2016-2020 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IVoxImporter.h"

/**
 * VoxImporter
 */
struct VoxExtensionImporter : public IVoxImporter
{
	using SceneGraphOperatorT = void(VoxExtensionImporter::*)(const vox::vox&, std::shared_ptr<vox::node>, const vox::translation&, const vox::rotation&);
	template <typename T> using TypeIndexMapT = TMap<uint32_t, T>;
	static const TypeIndexMapT<SceneGraphOperatorT> SceneGraphOperator;

public:
	/** Construct importer use parent voxel reference */
	VoxExtensionImporter(FVoxel* voxel);

	/** Import voxel model from vox object */
	void Import(const vox::vox& vox) override;

protected:
	/** Visit scene graph */
	void Visit(const vox::vox& vox, std::shared_ptr<vox::node> node, const vox::translation& translation, const vox::rotation& rotation);
	/** Visit transform node (nTRN) */
	void Transform(const vox::vox& vox, std::shared_ptr<vox::node> node, const vox::translation& translation, const vox::rotation& rotation);
	/** Visit group node (nGRP) */
	void Group(const vox::vox& vox, std::shared_ptr<vox::node> node, const vox::translation& translation, const vox::rotation& rotation);
	/** Visit shape node (nSHP) */
	void Shape(const vox::vox& vox, std::shared_ptr<vox::node> node, const vox::translation& translation, const vox::rotation& rotation);

private:
	/** Transform translation */
	static vox::translation Transform(const vox::rotation& M, const vox::translation& T);
	/** Transform vector */
	static FIntVector Transform(const vox::rotation& M, const FIntVector& V);

private: // Immutable members
	/** Scene graph depth */
	int Depth;
};
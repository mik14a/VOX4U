// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "VoxImportOption.generated.h"

/** Import mesh type */
UENUM()
enum class EVoxImportType
{
	StaticMesh UMETA(DisplayName = "Static Mesh"),
	Voxel UMETA(DisplayName = "Voxel")
};

/**
 *
 */
UCLASS(config = EditorPerProjectUserSettings, AutoExpandCategories = (FTransform), HideCategories = Object)
class UVoxImportOption : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = ImportType, meta = (ImportCategory = "ImportType"))
	TEnumAsByte<EVoxImportType> VoxImportType;

	UPROPERTY(EditAnywhere, Category = Transform, meta = (ImportType = "StaticMesh|Voxel", ImportCategory = "Transform"))
	uint32 bImportXForward : 1;

	UPROPERTY(EditAnywhere, Category = Transform, meta = (ImportType = "StaticMesh|Voxel", ImportCategory = "Transform"))
	uint32 bImportXYCenter : 1;

	UPROPERTY(EditAnywhere, Category = Transform, meta = (ImportType = "StaticMesh|Voxel", ImportCategory = "Transform"))
	FMeshBuildSettings BuildSettings;

	UPROPERTY(EditAnywhere, Category = Mesh, meta = (ImportType = "StaticMesh", ImportCategory = "Mesh"))
	uint32 bMergeVertexes : 1;

public:

	UVoxImportOption();

};

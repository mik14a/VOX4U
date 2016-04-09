// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "VoxImportOption.generated.h"

/** Import mesh type */
UENUM()
enum class EVoxImportType
{
	StaticMesh UMETA(DisplayName = "Static Mesh"),
	SkeletalMesh UMETA(DisplayName = "Skeletal Mesh"),
	Voxel UMETA(DisplayName = "Voxel"),
	MeshedVoxel UMETA(DisplayName = "Meshed Voxel")
};

/**
 *
 */
UCLASS(config = EditorPerProjectUserSettings, HideCategories = Object)
class UVoxImportOption : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = ImportType)
	TEnumAsByte<EVoxImportType> VoxImportType;

	UPROPERTY(EditAnywhere, Category = Generic)
	uint32 bImportXForward : 1;

	UPROPERTY(EditAnywhere, Category = Generic)
	uint32 bImportXYCenter : 1;

	UPROPERTY(EditAnywhere, Category = StaticMesh)
	FMeshBuildSettings BuildSettings;

	UPROPERTY(EditAnywhere, Category = "StaticMesh|Voxel")
	uint32 bMergeVertexes : 1;

	UPROPERTY(EditAnywhere, Category = StaticMesh)
	class UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, Category = Voxel)
	class UStaticMesh* Mesh;

public:

	UVoxImportOption();

	bool GetImportOption(bool& bImportAll);

};

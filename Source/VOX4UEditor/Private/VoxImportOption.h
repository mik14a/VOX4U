// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Engine/EngineTypes.h>
#include <UObject/NoExportTypes.h>
#include "VoxImportOption.generated.h"

/** Import mesh type */
UENUM()
enum class EVoxImportType
{
	StaticMesh UMETA(DisplayName = "Static Mesh"),
	SkeletalMesh UMETA(DisplayName = "Skeletal Mesh"),
	DestructibleMesh UMETA(DisplayName = "Destructible Mesh"),
	Voxel UMETA(DisplayName = "Voxel"),
};

/**
 * Import option
 */
UCLASS(config = EditorPerProjectUserSettings, HideCategories = Object)
class UVoxImportOption : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = ImportType)
	EVoxImportType VoxImportType;

	UPROPERTY(EditAnywhere, Category = Generic)
	uint32 bImportXForward : 1;

	UPROPERTY(EditAnywhere, Category = Generic)
	uint32 bImportXYCenter : 1;

	UPROPERTY(EditAnywhere, Category = Generic)
	float Scale;

public:

	UVoxImportOption();

	bool GetImportOption(bool& bOutImportAll);

	const FMeshBuildSettings& GetBuildSettings() const { 
		return BuildSettings;
	}

private:

	FMeshBuildSettings BuildSettings;

	friend class UVoxAssetImportData;

};

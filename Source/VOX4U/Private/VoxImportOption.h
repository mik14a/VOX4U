// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "VoxImportOption.generated.h"

/**
 *
 */
UCLASS(config = EditorPerProjectUserSettings, AutoExpandCategories = (FTransform), HideCategories = Object)
class UVoxImportOption : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Transform, meta = (ImportType = "StaticMesh|SkeletalMesh|Animation", ImportCategory = "Transform"))
	uint32 bImportXForward : 1;

	UPROPERTY(EditAnywhere, Category = Transform, meta = (ImportType = "StaticMesh|SkeletalMesh|Animation", ImportCategory = "Transform"))
	uint32 bImportXYCenter : 1;

	UPROPERTY(EditAnywhere, Category = Transform, meta = (ImportType = "StaticMesh|SkeletalMesh|Animation", ImportCategory = "Transform"))
	float ImportUniformScale;

public:

	UVoxImportOption();

};

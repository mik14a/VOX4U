// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <EditorFramework/AssetImportData.h>
#include "VoxImportOption.h"
#include "VoxAssetImportData.generated.h"

/**
 * Asset import data
 */
UCLASS()
class UVoxAssetImportData : public UAssetImportData
{
	GENERATED_BODY()

public:

	EVoxImportType VoxImportType;

	UPROPERTY(EditAnywhere, Category = Generic)
	uint32 bImportXForward : 1;

	UPROPERTY(EditAnywhere, Category = Generic)
	uint32 bImportXYCenter : 1;

	UPROPERTY(EditAnywhere, Category = Generic)
	float Scale;

public:

	UVoxAssetImportData();

	void ToVoxImportOption(UVoxImportOption& OutVoxImportOption);

	void FromVoxImportOption(const UVoxImportOption& VoxImportOption);

};

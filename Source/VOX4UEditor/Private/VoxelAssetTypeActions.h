// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <AssetTypeActions_Base.h>

class FVoxelAssetTypeActions : public FAssetTypeActions_Base
{
public:
	FVoxelAssetTypeActions();

	virtual FText GetName() const override;

	virtual UClass* GetSupportedClass() const override;

	virtual FColor GetTypeColor() const override;

	virtual uint32 GetCategories() override;

	virtual bool IsImportedAsset() const override;

};

// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "ActorFactories/ActorFactory.h"
#include "VoxelActorFactory.generated.h"

/**
 *
 */
UCLASS()
class UVoxelActorFactory : public UActorFactory
{
	GENERATED_UCLASS_BODY()

	virtual void PostSpawnActor(UObject* Asset, AActor* NewActor) override;

	virtual void PostCreateBlueprint(UObject* Asset, AActor* CDO) override;

	virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;

};

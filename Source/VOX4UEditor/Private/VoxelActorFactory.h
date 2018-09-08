// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <ActorFactories/ActorFactory.h>
#include "VoxelActorFactory.generated.h"

/**
 * Actor factory
 */
UCLASS()
class UVoxelActorFactory : public UActorFactory
{
	GENERATED_BODY()

	UVoxelActorFactory(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostSpawnActor(UObject* Asset, AActor* NewActor) override;

	virtual void PostCreateBlueprint(UObject* Asset, AActor* CDO) override;

	virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;

};

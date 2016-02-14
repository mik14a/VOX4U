// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "VoxelActor.generated.h"

UCLASS()
class AVoxelActor : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VoxelActor)
	class UVoxelComponent* VoxelComponent;

public:

	AVoxelActor();

public:

	class UVoxelComponent* GetVoxelComponent() const;

};

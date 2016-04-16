// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "UObjectGlobals.h"
#include "GameFramework/Actor.h"
#include "VoxelActor.generated.h"

class UVoxelComponent;

UCLASS()
class VOX4U_API AVoxelActor : public AActor
{
	GENERATED_BODY()

	AVoxelActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:

	UVoxelComponent* GetVoxelComponent() const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VoxelActor)
	UVoxelComponent* VoxelComponent;

};

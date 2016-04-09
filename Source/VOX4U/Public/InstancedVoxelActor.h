// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "UObjectGlobals.h"
#include "GameFramework/Actor.h"
#include "InstancedVoxelActor.generated.h"

class UInstancedVoxelComponent;

UCLASS()
class VOX4U_API AInstancedVoxelActor : public AActor
{
	GENERATED_BODY()

	AInstancedVoxelActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:

	UInstancedVoxelComponent* GetVoxelComponent() const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VoxelActor)
	UInstancedVoxelComponent* VoxelComponent;

};

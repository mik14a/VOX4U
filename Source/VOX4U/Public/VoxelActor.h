// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "UObjectGlobals.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/DestructibleActor.h"
#include "VoxelActor.generated.h"

class UVoxelComponent;

UCLASS()
class VOX4U_API AVoxelActor : public ADestructibleActor
{
	GENERATED_BODY()

	AVoxelActor(const FObjectInitializer& ObjectInitializer);

public:

	FORCEINLINE UVoxelComponent* GetVoxelComponent() const;

};

// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "MeshedVoxelActor.generated.h"

UCLASS()
class AMeshedVoxelActor : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VoxelActor)
	class UMeshedVoxelComponent* MeshedVoxelComponent;

public:

	AMeshedVoxelActor();

public:

	class UMeshedVoxelComponent* GetMeshedVoxelComponent() const;

};

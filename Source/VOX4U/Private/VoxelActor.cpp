// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "VoxelActor.h"
#include "VoxelComponent.h"
#include "Voxel.h"

AVoxelActor::AVoxelActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UVoxelComponent>(TEXT("DestructibleComponent0")))
{
}

UVoxelComponent* AVoxelActor::GetVoxelComponent() const
{
	return (UVoxelComponent*)GetDestructibleComponent();
}

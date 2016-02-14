// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "VoxelActor.h"
#include "VoxelComponent.h"

AVoxelActor::AVoxelActor()
{
	VoxelComponent = CreateDefaultSubobject<UVoxelComponent>("VoxelComponent");
	VoxelComponent->AttachTo(RootComponent);
}

class UVoxelComponent* AVoxelActor::GetVoxelComponent() const
{
	return VoxelComponent;
}

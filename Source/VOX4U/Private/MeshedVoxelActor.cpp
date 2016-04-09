// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "MeshedVoxelActor.h"
#include "MeshedVoxelComponent.h"

AMeshedVoxelActor::AMeshedVoxelActor()
{
	VoxelComponent = CreateDefaultSubobject<UMeshedVoxelComponent>("VoxelComponent");
	VoxelComponent->AttachTo(RootComponent);
}

class UMeshedVoxelComponent* AMeshedVoxelActor::GetVoxelComponent() const
{
	return VoxelComponent;
}

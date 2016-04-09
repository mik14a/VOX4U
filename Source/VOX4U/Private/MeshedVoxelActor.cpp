// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "MeshedVoxelActor.h"
#include "MeshedVoxelComponent.h"

AMeshedVoxelActor::AMeshedVoxelActor()
{
	MeshedVoxelComponent = CreateDefaultSubobject<UMeshedVoxelComponent>("VoxelComponent");
	MeshedVoxelComponent->AttachTo(RootComponent);
}

class UMeshedVoxelComponent* AMeshedVoxelActor::GetMeshedVoxelComponent() const
{
	return MeshedVoxelComponent;
}

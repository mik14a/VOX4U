// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "VoxelActor.h"
#include "VoxelComponent.h"
#include "Voxel.h"

AVoxelActor::AVoxelActor(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	VoxelComponent = CreateDefaultSubobject<UVoxelComponent>("VoxelComponent");
	VoxelComponent->AttachTo(RootComponent);
}

UVoxelComponent* AVoxelActor::GetVoxelComponent() const
{
	return VoxelComponent;
}

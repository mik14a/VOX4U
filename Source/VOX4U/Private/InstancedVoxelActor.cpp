// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "InstancedVoxelActor.h"
#include "InstancedVoxelComponent.h"
#include "InstancedVoxel.h"

AInstancedVoxelActor::AInstancedVoxelActor(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	VoxelComponent = CreateDefaultSubobject<UInstancedVoxelComponent>("VoxelComponent");
	VoxelComponent->AttachTo(RootComponent);
}

UInstancedVoxelComponent* AInstancedVoxelActor::GetVoxelComponent() const
{
	return VoxelComponent;
}

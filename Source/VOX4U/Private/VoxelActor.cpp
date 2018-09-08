// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#include "VoxelActor.h"
#include "VoxelComponent.h"

AVoxelActor::AVoxelActor(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	VoxelComponent = CreateDefaultSubobject<UVoxelComponent>("VoxelComponent");
	VoxelComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

UVoxelComponent* AVoxelActor::GetVoxelComponent() const
{
	return VoxelComponent;
}

// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "PrimitiveSceneProxy.h"

class UVoxelComponent;

/**
 *
 */
class FVoxelSceneProxy : public FPrimitiveSceneProxy
{
public:
	FVoxelSceneProxy(UVoxelComponent* Component);
	virtual ~FVoxelSceneProxy() {}
};

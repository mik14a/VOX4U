// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "Object.h"
#include "IntVoxel.generated.h"

/**
 * VOXEL Chunk
 */
USTRUCT()
struct FIntVoxel : public FIntVector
{
	GENERATED_BODY()

	/** VOXEL palette index */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FIntVoxel)
	int32 I;

public:

	FIntVoxel() { }

	FIntVoxel(int32 InX, int32 InY, int32 InZ, int32 InI) : FIntVector(InX, InY, InZ), I(InI) { }

	explicit FORCEINLINE FIntVoxel(EForceInit) : FIntVector(ForceInit), I(0) { }

};

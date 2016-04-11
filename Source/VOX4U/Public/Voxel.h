// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "UObject.h"
#include "Object.h"
#include "UnrealTemplate.h"
#include "IntVoxel.h"
#include "Voxel.generated.h"

class UDestructibleMesh;

/**
 * VOXEL Asset
 */
UCLASS()
class VOX4U_API UVoxel : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Voxel)
	FIntVector Size;

	UPROPERTY(EditDefaultsOnly, Category = Voxel)
	uint32 bXYCenter : 1;

	UPROPERTY(EditDefaultsOnly, EditFixedSize, Category = Voxel)
	UDestructibleMesh* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = Voxel)
	TArray<FIntVoxel> Voxel;

public:

	UVoxel();

};

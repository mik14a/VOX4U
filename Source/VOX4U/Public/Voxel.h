// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "UObject.h"
#include "Object.h"
#include "UnrealTemplate.h"
#include "IntVoxel.h"
#include "Voxel.generated.h"

class UStaticMesh;

/**
 * VOXEL Asset
 */
UCLASS()
class UVoxel : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Voxel)
	FIntVector Size;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Voxel)
	FBoxSphereBounds CellBounds;

	UPROPERTY(EditDefaultsOnly, Category = Voxel)
	uint32 bXYCenter : 1;

	UPROPERTY(EditDefaultsOnly, EditFixedSize, Category = Voxel)
	TArray<UStaticMesh*> Mesh;

	UPROPERTY(EditDefaultsOnly, Category = Voxel)
	TArray<FIntVoxel> Voxel;

public:

	UVoxel();

#if WITH_EDITOR

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void CalcCellBounds();

#endif // WITH_EDITOR

};

// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "VoxelComponent.generated.h"

/**
 *
 */
UCLASS()
class UVoxelComponent : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:

	//virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	void SetVoxel(class UVoxel* InVoxel) {
		Voxel = InVoxel;
	}

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VoxelComponent)
	class UVoxel* Voxel;

};

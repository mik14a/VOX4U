// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "Components/PrimitiveComponent.h"
#include "MeshedVoxelComponent.generated.h"

/**
 *
 */
UCLASS()
class UMeshedVoxelComponent : public UMeshComponent
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

	virtual int32 GetNumMaterials() const override;

	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	void SetVoxel(class UMeshedVoxel* InVoxel) {
		if (Voxel == InVoxel) return;
		Voxel = InVoxel;
	}

	UMeshedVoxel* GetVoxel() const {
		return Voxel;
	}

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VoxelComponent)
	class UMeshedVoxel* Voxel;

};

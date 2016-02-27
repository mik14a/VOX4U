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

	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	void SetVoxel(class UVoxel* InVoxel) {
		if (Voxel == InVoxel) return;
		Voxel = InVoxel;
		ClearInstances();
		if (Voxel) {
			AddVoxelWorldSpace();
		}
	}

private:

	void AddVoxelWorldSpace();

	bool IsUnbeheldVolume(const FIntVector& Cell) const;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VoxelComponent)
	class UVoxel* Voxel;

};

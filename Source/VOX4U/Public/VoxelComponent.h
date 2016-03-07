// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "IntVoxel.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "VoxelComponent.generated.h"

class UVoxel;

UCLASS()
class UInstancedVoxelComponent : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()
};

/**
 *
 */
UCLASS()
class UVoxelComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VoxelComponent)
	FBoxSphereBounds CellBounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VoxelComponent)
	bool bHideUnbeheld;

	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite, Category = VoxelComponent)
	TArray<UStaticMesh*> Mesh;

	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite, Category = VoxelComponent)
	TArray<FIntVoxel> Cell;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VoxelComponent)
	UVoxel* Voxel;

public:

	UVoxelComponent();

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif // WITH_EDITOR

	void SetVoxel(class UVoxel* InVoxel, bool bForce = false);

	UFUNCTION(BlueprintCallable, Category = Voxel)
	void AddVoxel();

	UFUNCTION(BlueprintCallable, Category = Voxel)
	void ClearVoxel();

	UFUNCTION(BlueprintCallable, Category = Voxel)
	bool IsUnbeheldVolume(const FIntVector& InVector) const;

	UFUNCTION(BlueprintCallable, Category = Voxel)
	bool GetVoxelTransform(int32 VoxelIndex, FTransform& OutVoxelTransform, bool bWorldSpace = false) const;

	const TArray<UInstancedVoxelComponent*>& GetVoxelComponents() const;

private:

	void InitVoxel();

protected:

	UPROPERTY()
	TArray<UInstancedVoxelComponent*> VoxelComponents;

};

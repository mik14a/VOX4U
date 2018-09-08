// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Components/PrimitiveComponent.h>
#include "VoxelComponent.generated.h"

class UInstancedStaticMeshComponent;
class UStaticMesh;
class UVoxel;

/**
 * Voxel component
 */
UCLASS()
class VOX4U_API UVoxelComponent : public UPrimitiveComponent
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
	TMap<FIntVector, uint8> Cell;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VoxelComponent)
	UVoxel* Voxel;

public:

	UVoxelComponent();

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif // WITH_EDITOR

	void SetVoxel(class UVoxel* InVoxel, bool bForce = false);

	const UVoxel* GetVoxel() const;

	UFUNCTION(BlueprintCallable, Category = Voxel)
	void AddVoxel();

	UFUNCTION(BlueprintCallable, Category = Voxel)
	void ClearVoxel();

	UFUNCTION(BlueprintCallable, Category = Voxel)
	bool IsUnbeheldVolume(const FIntVector& InVector) const;

	UFUNCTION(BlueprintCallable, Category = Voxel)
	bool GetVoxelTransform(const FIntVector& InVector, FTransform& OutVoxelTransform, bool bWorldSpace = false) const;

	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	const TArray<UInstancedStaticMeshComponent*>& GetInstancedStaticMeshComponent() const;

private:

	void InitVoxel();

protected:

	UPROPERTY()
	TArray<UInstancedStaticMeshComponent*> InstancedStaticMeshComponents;

};

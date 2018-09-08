// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <UObject/NoExportTypes.h>
#include "Voxel.generated.h"

class UStaticMesh;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Voxel)
	FBoxSphereBounds CellBounds;

	UPROPERTY(EditDefaultsOnly, Category = Voxel)
	uint32 bXYCenter : 1;

	UPROPERTY(EditDefaultsOnly, EditFixedSize, Category = Voxel)
	TArray<UStaticMesh*> Mesh;

	UPROPERTY(EditDefaultsOnly, Category = Voxel)
	TMap<FIntVector, uint8> Voxel;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Instanced, Category = Reimport)
	class UAssetImportData* AssetImportData;
#endif

public:

	UVoxel();

#if WITH_EDITOR

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void CalcCellBounds();

#endif // WITH_EDITOR

};

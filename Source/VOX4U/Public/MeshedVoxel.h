// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "MeshedVoxel.generated.h"

class UMaterialInterface;

UCLASS()
class UMeshedVoxel : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Voxel)
	FIntVector Size;

	UPROPERTY(EditDefaultsOnly, Category = Voxel)
	uint32 bXYCenter : 1;

	// UPROPERTY()
	// UHT Error: USTRUCTs are not currently supported as key types.
	TMap<FIntVector, uint8> Voxel;

	UPROPERTY(EditDefaultsOnly, Category = Voxel)
	UMaterialInterface* Material;

public:

	UMeshedVoxel();

	virtual void Serialize(FArchive& Ar) override;

};

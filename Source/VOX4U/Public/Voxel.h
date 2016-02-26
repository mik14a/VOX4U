// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "UObject.h"
#include "Object.h"
#include "UnrealTemplate.h"
#include "Voxel.generated.h"

UCLASS()
class UVoxel : public UObject
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
	class UStaticMesh* Mesh;

public:

	UVoxel();

	virtual void Serialize(FArchive& Ar) override;

};

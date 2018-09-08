// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#include "VoxelActorFactory.h"
#include <AssetData.h>
#include "Voxel.h"
#include "VoxelActor.h"
#include "VoxelComponent.h"

UVoxelActorFactory::UVoxelActorFactory(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	DisplayName = NSLOCTEXT("VOX4U", "UVoxelActorFactory", "Voxel Actor");
	NewActorClass = AVoxelActor::StaticClass();
}

void UVoxelActorFactory::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);

	AVoxelActor* VoxelActor = CastChecked<AVoxelActor>(NewActor);
	UVoxelComponent* VoxelComponent = VoxelActor->GetVoxelComponent();
	check(VoxelComponent);

	if (UVoxel* Voxel = Cast<UVoxel>(Asset)) {
		VoxelComponent->UnregisterComponent();
		VoxelComponent->SetVoxel(Voxel, true);
		VoxelComponent->RegisterComponent();
	}
	VoxelActor->PostEditChange();
}

void UVoxelActorFactory::PostCreateBlueprint(UObject* Asset, AActor* CDO)
{
	if (AVoxelActor* VoxelActor = Cast<AVoxelActor>(CDO)) {
		UVoxelComponent* VoxelComponent = VoxelActor->GetVoxelComponent();
		check(VoxelComponent);
		if (UVoxel* Voxel = Cast<UVoxel>(Asset)) {
			VoxelComponent->SetVoxel(Voxel, true);
		}
	}
}

bool UVoxelActorFactory::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if (AssetData.IsValid()) {
		UClass* AssetClass = AssetData.GetClass();
		if (AssetClass && AssetClass->IsChildOf(UVoxel::StaticClass())) {
			return true;
		} else {
			OutErrorMsg = NSLOCTEXT("VOX4U", "CanCreateActorFrom_NoVoxel", "No Voxel data specified.");
			return false;
		}
	} else {
		return true;
	}
}

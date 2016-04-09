// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "MeshedVoxelActorFactory.h"
#include "Voxel.h"
#include "MeshedVoxelActor.h"
#include "MeshedVoxelComponent.h"

UMeshedVoxelActorFactory::UMeshedVoxelActorFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = NSLOCTEXT("VOX4U", "UVoxelActorFactory", "Voxel Actor");
	NewActorClass = AMeshedVoxelActor::StaticClass();
}

void UMeshedVoxelActorFactory::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);

	AMeshedVoxelActor* VoxelActor = CastChecked<AMeshedVoxelActor>(NewActor);
	UMeshedVoxelComponent* VoxelComponent = VoxelActor->GetMeshedVoxelComponent();
	check(VoxelComponent);

	if (UVoxel* Voxel = Cast<UVoxel>(Asset)) {
		VoxelComponent->UnregisterComponent();
		VoxelComponent->SetVoxel(Voxel);
		VoxelComponent->RegisterComponent();
	}
}

void UMeshedVoxelActorFactory::PostCreateBlueprint(UObject* Asset, AActor* CDO)
{
	if (AMeshedVoxelActor* VoxelActor = Cast<AMeshedVoxelActor>(CDO)) {
		UMeshedVoxelComponent* VoxelComponent = VoxelActor->GetMeshedVoxelComponent();
		check(VoxelComponent);
		if (UVoxel* Voxel = Cast<UVoxel>(Asset)) {
			VoxelComponent->SetVoxel(Voxel);
		}
	}
}

bool UMeshedVoxelActorFactory::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
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

// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "MeshedVoxelActorFactory.h"
#include "MeshedVoxel.h"
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

	AMeshedVoxelActor* MeshedVoxelActor = CastChecked<AMeshedVoxelActor>(NewActor);
	UMeshedVoxelComponent* MeshedVoxelComponent = MeshedVoxelActor->GetMeshedVoxelComponent();
	check(MeshedVoxelComponent);

	if (UMeshedVoxel* MeshedVoxel = Cast<UMeshedVoxel>(Asset)) {
		MeshedVoxelComponent->UnregisterComponent();
		MeshedVoxelComponent->SetVoxel(MeshedVoxel);
		MeshedVoxelComponent->RegisterComponent();
	}
}

void UMeshedVoxelActorFactory::PostCreateBlueprint(UObject* Asset, AActor* CDO)
{
	if (AMeshedVoxelActor* MeshedVoxelActor = Cast<AMeshedVoxelActor>(CDO)) {
		UMeshedVoxelComponent* MeshedVoxelComponent = MeshedVoxelActor->GetMeshedVoxelComponent();
		check(MeshedVoxelComponent);
		if (UMeshedVoxel* MeshedVoxel = Cast<UMeshedVoxel>(Asset)) {
			MeshedVoxelComponent->SetVoxel(MeshedVoxel);
		}
	}
}

bool UMeshedVoxelActorFactory::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if (AssetData.IsValid()) {
		UClass* AssetClass = AssetData.GetClass();
		if (AssetClass && AssetClass->IsChildOf(UMeshedVoxel::StaticClass())) {
			return true;
		} else {
			OutErrorMsg = NSLOCTEXT("VOX4U", "CanCreateActorFrom_NoVoxel", "No Voxel data specified.");
			return false;
		}
	} else {
		return true;
	}
}

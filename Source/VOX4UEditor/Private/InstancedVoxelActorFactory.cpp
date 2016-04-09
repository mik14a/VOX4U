// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UEditorPrivatePCH.h"
#include "InstancedVoxelActorFactory.h"
#include "InstancedVoxel.h"
#include "InstancedVoxelActor.h"
#include "InstancedVoxelComponent.h"

UInstancedVoxelActorFactory::UInstancedVoxelActorFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = NSLOCTEXT("VOX4U", "UVoxelActorFactory", "Voxel Actor");
	NewActorClass = AInstancedVoxelActor::StaticClass();
}

void UInstancedVoxelActorFactory::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);

	AInstancedVoxelActor* VoxelActor = CastChecked<AInstancedVoxelActor>(NewActor);
	UInstancedVoxelComponent* VoxelComponent = VoxelActor->GetVoxelComponent();
	check(VoxelComponent);

	if (UInstancedVoxel* Voxel = Cast<UInstancedVoxel>(Asset)) {
		VoxelComponent->UnregisterComponent();
		VoxelComponent->SetVoxel(Voxel, true);
		VoxelComponent->RegisterComponent();
	}
}

void UInstancedVoxelActorFactory::PostCreateBlueprint(UObject* Asset, AActor* CDO)
{
	if (AInstancedVoxelActor* VoxelActor = Cast<AInstancedVoxelActor>(CDO)) {
		UInstancedVoxelComponent* VoxelComponent = VoxelActor->GetVoxelComponent();
		check(VoxelComponent);
		if (UInstancedVoxel* Voxel = Cast<UInstancedVoxel>(Asset)) {
			VoxelComponent->SetVoxel(Voxel, true);
		}
	}
}

bool UInstancedVoxelActorFactory::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if (AssetData.IsValid()) {
		UClass* AssetClass = AssetData.GetClass();
		if (AssetClass && AssetClass->IsChildOf(UInstancedVoxel::StaticClass())) {
			return true;
		} else {
			OutErrorMsg = NSLOCTEXT("VOX4U", "CanCreateActorFrom_NoVoxel", "No Voxel data specified.");
			return false;
		}
	} else {
		return true;
	}
}

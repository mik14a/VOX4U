// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "VoxelComponent.h"
#include "Voxel.h"
#include "ArrayBuilder.h"
#include "StaticMeshResources.h"
#include "VoxelSceneProxy.h"


void UVoxelComponent::BeginPlay()
{
}

#if WITH_EDITOR
void UVoxelComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	static const FName NAME_Voxel = FName(TEXT("Voxel"));
	if (PropertyChangedEvent.Property) {
		if (PropertyChangedEvent.Property->GetFName() == NAME_Voxel) {
			SetMaterial(0, Voxel ? Voxel->Material : nullptr);
		}
	}
}
#endif // WITH_EDITOR

FPrimitiveSceneProxy* UVoxelComponent::CreateSceneProxy()
{
	return Voxel ? new FVoxelSceneProxy(this) : nullptr;
}

int32 UVoxelComponent::GetNumMaterials() const
{
	return 1;
}

FBoxSphereBounds UVoxelComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds();
}

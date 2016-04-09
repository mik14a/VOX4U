// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "MeshedVoxelComponent.h"
#include "MeshedVoxel.h"
#include "ArrayBuilder.h"
#include "StaticMeshResources.h"
#include "MeshedVoxelSceneProxy.h"


void UMeshedVoxelComponent::BeginPlay()
{
}

#if WITH_EDITOR
void UMeshedVoxelComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	static const FName NAME_Voxel = FName(TEXT("Voxel"));
	if (PropertyChangedEvent.Property) {
		if (PropertyChangedEvent.Property->GetFName() == NAME_Voxel) {
			SetMaterial(0, Voxel ? Voxel->Material : nullptr);
		}
	}
}
#endif // WITH_EDITOR

FPrimitiveSceneProxy* UMeshedVoxelComponent::CreateSceneProxy()
{
	return Voxel ? new FMeshedVoxelSceneProxy(this) : nullptr;
}

int32 UMeshedVoxelComponent::GetNumMaterials() const
{
	return 1;
}

FBoxSphereBounds UMeshedVoxelComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds(ForceInit);
}

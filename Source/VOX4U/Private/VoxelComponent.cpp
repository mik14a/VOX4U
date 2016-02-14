// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "VoxelComponent.h"
#include "Voxel.h"

#if WITH_EDITOR
void UVoxelComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	static const FName NAME_Voxel = FName(TEXT("Voxel"));

	if (PropertyChangedEvent.Property != NULL) {
		if (PropertyChangedEvent.Property->GetFName() == NAME_Voxel) {
			ClearInstances();
			if (Voxel) {
				FVector Offset = Voxel->bXYCenter ? FVector((float)Voxel->Size.X / 2, (float)Voxel->Size.Y / 2, 0.f) : FVector::ZeroVector;
				for (auto& voxel : Voxel->Voxel) {
					FVector Translation = FVector(voxel.Key.X, voxel.Key.Y, voxel.Key.Z) - Offset;
					FTransform Transform(FQuat::Identity, Translation, FVector(1.f));
					AddInstance(Transform);
				}
			}
		}
	}
}
#endif // WITH_EDITOR

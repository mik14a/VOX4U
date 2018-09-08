// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#include "VoxelThumbnailRenderer.h"
#include <EngineModule.h>
#include <LegacyScreenPercentageDriver.h>
#include <ThumbnailRendering/SceneThumbnailInfo.h>
#include "Voxel.h"
#include "VoxelActor.h"
#include "VoxelComponent.h"

FVoxelThumbnailScene::FVoxelThumbnailScene()
{
	bForceAllUsedMipsResident = false;
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.bNoFail = true;
	SpawnInfo.ObjectFlags = RF_Transient;
	Actor = GetWorld()->SpawnActor<AVoxelActor>(SpawnInfo);
	Actor->GetVoxelComponent()->SetMobility(EComponentMobility::Movable);
	Actor->SetActorEnableCollision(false);
}

void FVoxelThumbnailScene::SetVoxel(UVoxel* Voxel)
{
	Actor->GetVoxelComponent()->SetVoxel(Voxel);
	if (Voxel) {
		Actor->SetActorLocation(FVector(0, 0, 0), false);
		Actor->GetVoxelComponent()->UpdateBounds();
		const float BoundsZOffset = GetBoundsZOffset(Actor->GetVoxelComponent()->Bounds);
		Actor->SetActorLocation(-Actor->GetVoxelComponent()->Bounds.Origin + FVector(0, 0, BoundsZOffset), false);
		Actor->GetVoxelComponent()->RecreateRenderState_Concurrent();
		Actor->PostEditChange();
	}
}

void FVoxelThumbnailScene::GetViewMatrixParameters(const float InFOVDegrees, FVector& OutOrigin, float& OutOrbitPitch, float& OutOrbitYaw, float& OutOrbitZoom) const
{
	check(Actor);
	check(Actor->GetVoxelComponent());
	check(Actor->GetVoxelComponent()->GetVoxel());

	const float HalfFOVRadians = FMath::DegreesToRadians<float>(InFOVDegrees) * 0.5f;
	const float HalfMeshSize = Actor->GetVoxelComponent()->Bounds.SphereRadius * 1.15;
	const float BoundsZOffset = GetBoundsZOffset(Actor->GetVoxelComponent()->Bounds);
	const float TargetDistance = HalfMeshSize / FMath::Tan(HalfFOVRadians);

	USceneThumbnailInfo* ThumbnailInfo = USceneThumbnailInfo::StaticClass()->GetDefaultObject<USceneThumbnailInfo>();
	OutOrigin = FVector(0, 0, -BoundsZOffset);
	OutOrbitPitch = ThumbnailInfo->OrbitPitch;
	OutOrbitYaw = ThumbnailInfo->OrbitYaw;
	OutOrbitZoom = TargetDistance + ThumbnailInfo->OrbitZoom;
}

void UVoxelThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* Viewport, FCanvas* Canvas)
{
	UVoxel* Voxel = Cast<UVoxel>(Object);
	if (Voxel && !Voxel->IsPendingKill()) {
		FVoxelThumbnailScene* ThumbnailScene = ThumbnailScenes.FindRef(Voxel);
		if (!ThumbnailScene) {
			ThumbnailScene = new FVoxelThumbnailScene();
			ThumbnailScene->SetVoxel(Voxel);
			ThumbnailScenes.Add(Voxel, ThumbnailScene);
		}
		ThumbnailScene->GetScene()->UpdateSpeedTreeWind(0.0);
		FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(Viewport, ThumbnailScene->GetScene(), FEngineShowFlags(ESFIM_Game))
			.SetWorldTimes(FApp::GetCurrentTime() - GStartTime, FApp::GetDeltaTime(), FApp::GetCurrentTime() - GStartTime));
		ViewFamily.EngineShowFlags.DisableAdvancedFeatures();
		ViewFamily.EngineShowFlags.MotionBlur = 0;
		ViewFamily.EngineShowFlags.LOD = 0;
		ViewFamily.EngineShowFlags.ScreenPercentage = false;
		ViewFamily.SetScreenPercentageInterface(new FLegacyScreenPercentageDriver(ViewFamily, 1.0f, false));
		ThumbnailScene->GetView(&ViewFamily, X, Y, Width, Height);
		GetRendererModule().BeginRenderingViewFamily(Canvas, &ViewFamily);
	}
}

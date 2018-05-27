// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "ThumbnailRendering/ThumbnailRenderer.h"
#include "ThumbnailRendering/DefaultSizedThumbnailRenderer.h"
#include "ThumbnailRendering/SceneThumbnailInfo.h"
#include "ThumbnailHelpers.h"
#include "VoxelThumbnailRenderer.generated.h"

class UVoxel;
class AVoxelActor;
class FRenderTarget;
class FCanvas;

class FVoxelThumbnailScene : public FThumbnailPreviewScene
{
public:

	FVoxelThumbnailScene();

	void SetVoxel(UVoxel* Voxel);

protected:

	virtual void GetViewMatrixParameters(const float InFOVDegrees, FVector& OutOrigin, float& OutOrbitPitch, float& OutOrbitYaw, float& OutOrbitZoom) const override;

private:

	AVoxelActor* Actor;

};

/**
 * Voxel asset thumbnail renderer
 */
UCLASS()
class UVoxelThumbnailRenderer : public UDefaultSizedThumbnailRenderer
{
	GENERATED_BODY()
	
public:

	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* Viewport, FCanvas* Canvas) override;

private:

	TMap<UVoxel*, FVoxelThumbnailScene*> ThumbnailScenes;

};

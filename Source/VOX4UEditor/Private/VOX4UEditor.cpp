// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UEditor.h"
#include "ThumbnailRendering/ThumbnailManager.h"
#include "Voxel.h"
#include "VoxelThumbnailRenderer.h"

#define LOCTEXT_NAMESPACE "FVOX4UEditorModule"

void FVOX4UEditorModule::StartupModule()
{
	UThumbnailManager::Get().RegisterCustomRenderer(UVoxel::StaticClass(), UVoxelThumbnailRenderer::StaticClass());
}

void FVOX4UEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVOX4UEditorModule, VOX4UEditor)

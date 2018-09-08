// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UEditor.h"
#include <AssetToolsModule.h>
#include <IAssetTools.h>
#include "Voxel.h"
#include "VoxelAssetTypeActions.h"
#include "VoxelThumbnailRenderer.h"

#define LOCTEXT_NAMESPACE "FVOX4UEditorModule"

void FVOX4UEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	VoxelAssetTypeActions = MakeShareable(new FVoxelAssetTypeActions());
	AssetTools.RegisterAssetTypeActions(VoxelAssetTypeActions.ToSharedRef());
	UThumbnailManager::Get().RegisterCustomRenderer(UVoxel::StaticClass(), UVoxelThumbnailRenderer::StaticClass());
}

void FVOX4UEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools")) {
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		AssetTools.UnregisterAssetTypeActions(VoxelAssetTypeActions.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVOX4UEditorModule, VOX4UEditor)

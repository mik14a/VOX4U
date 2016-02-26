// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "VoxImportOption.h"
#include "Interfaces/IMainFrameModule.h"
#include "MainFrame.h"
#include "SlateCore.h"
#include "VoxOptionWidget.h"

UVoxImportOption::UVoxImportOption()
	: VoxImportType(EVoxImportType::StaticMesh)
	, bImportXForward(true)
	, bImportXYCenter(true)
	, bMergeVertexes(true)
	, Material(nullptr)
	, Mesh(nullptr)
{
	BuildSettings.bUseMikkTSpace = false;
	BuildSettings.BuildScale3D = FVector(10.f);
}

bool UVoxImportOption::GetImportOption(bool& bImportAll)
{
	TSharedPtr<SWindow> ParentWindow;

 	if (FModuleManager::Get().IsModuleLoaded("MainFrame")) {
		IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
		ParentWindow = MainFrame.GetParentWindow();
	}

	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(NSLOCTEXT("VOX4U", "VoxImportOpionsTitle", "VOX Import Options"))
		.SizingRule(ESizingRule::Autosized);

	TSharedPtr<SVoxOptionWidget> VoxOptionWidget;
	Window->SetContent(SAssignNew(VoxOptionWidget, SVoxOptionWidget).Window(Window).ImportOption(this));
	FSlateApplication::Get().AddModalWindow(Window, ParentWindow, false);
	SaveConfig();

	bImportAll = VoxOptionWidget->ShouldImportAll();

	return VoxOptionWidget->ShouldImport();
}

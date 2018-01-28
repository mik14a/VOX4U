// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VoxImportOption.h"
#include "Interfaces/IMainFrameModule.h"
#include "MainFrame.h"
#include "SlateCore.h"
#include "VoxOptionWidget.h"

UVoxImportOption::UVoxImportOption()
	: VoxImportType(EVoxImportType::StaticMesh)
	, bImportXForward(true)
	, bImportXYCenter(true)
	, Scale(10.f)
{
	BuildSettings.BuildScale3D = FVector(Scale);
}

bool UVoxImportOption::GetImportOption(bool& bOutImportAll)
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

	BuildSettings.BuildScale3D = FVector(Scale);
	bOutImportAll = VoxOptionWidget->ShouldImportAll();

	return VoxOptionWidget->ShouldImport();
}

// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#include "SVoxOptionWidget.h"
#include <Modules/ModuleManager.h>
#include <PropertyEditorModule.h>
#include <SlateOptMacros.h>
#include <Widgets/Input/SButton.h>
#include <Widgets/Layout/SBox.h>
#include <Widgets/Layout/SUniformGridPanel.h>
#include "VoxImportOption.h"

SVoxOptionWidget::SVoxOptionWidget()
	: Window(nullptr)
	, ImportOption(nullptr)
{
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SVoxOptionWidget::Construct(const FArguments& InArgs)
{
	ImportOption = InArgs._ImportOption;
	Window = InArgs._Window;

	TSharedPtr<SBox> InspectorBox;

	this->ChildSlot[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight().Padding(2) [
			SAssignNew(InspectorBox, SBox).MaxDesiredHeight(650.0f).WidthOverride(400.0f)
		]
		+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Right).Padding(2)
		[
			SNew(SUniformGridPanel).SlotPadding(2)
			+ SUniformGridPanel::Slot(0, 0)[
				SNew(SButton).HAlign(HAlign_Center).Text(NSLOCTEXT("VOX4U", "VoxOptionWindow_ImportAll", "Import All"))
				.OnClicked(this, &SVoxOptionWidget::OnImportAll)
			]
			+ SUniformGridPanel::Slot(1, 0) [
				SNew(SButton).HAlign(HAlign_Center).Text(NSLOCTEXT("VOX4U", "VoxOptionWindow_Import", "Import"))
				.OnClicked(this, &SVoxOptionWidget::OnImport)
			]
			+ SUniformGridPanel::Slot(2, 0) [
				SNew(SButton).HAlign(HAlign_Center).Text(NSLOCTEXT("VOX4U", "VoxOptionWindow_Cancel", "Cancel"))
				.OnClicked(this, &SVoxOptionWidget::OnCancel)
			]
		]
	];

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	InspectorBox->SetContent(DetailsView->AsShared());
	DetailsView->SetObject(ImportOption);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SVoxOptionWidget::OnImportAll()
{
	bShouldImportAll = true;
	return OnImport();
}

FReply SVoxOptionWidget::OnImport()
{
	bShouldImport = true;
	if (Window.IsValid()) {
		Window.Pin()->RequestDestroyWindow();
	}
	return FReply::Handled();
}

FReply SVoxOptionWidget::OnCancel()
{
	bShouldImport = false;
	bShouldImportAll = false;
	if (Window.IsValid()) {
		Window.Pin()->RequestDestroyWindow();
	}
	return FReply::Handled();
}

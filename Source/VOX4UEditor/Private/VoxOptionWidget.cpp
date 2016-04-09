// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UEditorPrivatePCH.h"
#include "VoxOptionWidget.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "VoxImportOption.h"
#include "SharedPointer.h"

SVoxOptionWidget::SVoxOptionWidget()
	: Window(nullptr)
	, ImportOption(nullptr)
{
}

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

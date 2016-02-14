// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "VoxFactory.h"
#include "Editor.h"
#include "Engine.h"
#include "Engine/StaticMesh.h"
#include "MainFrame.h"
#include "RawMesh.h"
#include "SlateCore.h"
#include "Vox.h"
#include "VoxImportOption.h"
#include "VoxOptionWidget.h"
#include "Interfaces/IMainFrameModule.h"

UVoxFactory::UVoxFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Formats.Add(TEXT("vox;MagicaVoxel"));

	bCreateNew = false;
	bText = false;
	bEditorImport = true;
}

void UVoxFactory::PostInitProperties()
{
	Super::PostInitProperties();
	ImportOption = NewObject<UVoxImportOption>(this, NAME_None, RF_NoFlags);
}

bool UVoxFactory::DoesSupportClass(UClass * Class)
{
	return Class == UStaticMesh::StaticClass();
}

UClass* UVoxFactory::ResolveSupportedClass()
{
	return UStaticMesh::StaticClass();
}

UObject* UVoxFactory::FactoryCreateBinary(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn)
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
	Window->SetContent(SAssignNew(VoxOptionWidget, SVoxOptionWidget).Window(Window).ImportOption(ImportOption));
	FSlateApplication::Get().AddModalWindow(Window, ParentWindow, false);
	ImportOption->SaveConfig();

	UStaticMesh* Result = nullptr;

	FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

	FBufferReader Reader((void*)Buffer, BufferEnd - Buffer, false);

	FVox Vox;
	Vox.Import(Reader, ImportOption);

	FRawMesh RawMesh;
	if (Vox.CreateRawMesh(RawMesh, ImportOption)) {

		Result = NewObject<UStaticMesh>(InParent, InName, Flags | RF_Public);
		for (int32 i = 0; i < RawMesh.FaceMaterialIndices.Num(); ++i) {
			Result->Materials.Add(UMaterial::GetDefaultMaterial(MD_Surface));
		}
		FStaticMeshSourceModel* StaticMeshSourceModel = new(Result->SourceModels) FStaticMeshSourceModel();
		StaticMeshSourceModel->RawMeshBulkData->SaveRawMesh(RawMesh);
	}

	FEditorDelegates::OnAssetPostImport.Broadcast(this, Result);

	return Result;
}

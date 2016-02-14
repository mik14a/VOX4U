// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "VoxFactory.h"
#include "Editor.h"
#include "Engine.h"
#include "Engine/StaticMesh.h"
#include "Interfaces/IMainFrameModule.h"
#include "MainFrame.h"
#include "RawMesh.h"
#include "SlateCore.h"
#include "Vox.h"
#include "VoxImportOption.h"
#include "VoxOptionWidget.h"
#include "Voxel.h"

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
	return Class == UStaticMesh::StaticClass()
		|| Class == UVoxel::StaticClass();
}

UClass* UVoxFactory::ResolveSupportedClass()
{
	UClass* Class = nullptr;
	if (ImportOption->VoxImportType == EVoxImportType::StaticMesh) {
		Class = UStaticMesh::StaticClass();
	} else if (ImportOption->VoxImportType == EVoxImportType::Voxel) {
		Class = UVoxel::StaticClass();
	}
	return Class;
}

UObject* UVoxFactory::FactoryCreateBinary(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn)
{
	FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

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

	FBufferReader Reader((void*)Buffer, BufferEnd - Buffer, false);
	FVox Vox(Reader, ImportOption);

	UObject* Result = nullptr;
	switch (ImportOption->VoxImportType) {
	case EVoxImportType::StaticMesh:
		Result = CreateStaticMesh(InParent, InName, Flags, &Vox);
		break;
	case EVoxImportType::Voxel:
		Result = CreateVoxel(InParent, InName, Flags, &Vox);
		break;
	default:
		break;
	}

	FEditorDelegates::OnAssetPostImport.Broadcast(this, Result);

	return Result;
}

UStaticMesh* UVoxFactory::CreateStaticMesh(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	UStaticMesh* StaticMesh = nullptr;
	FRawMesh RawMesh;
	if (Vox->CreateRawMesh(RawMesh, ImportOption)) {
		StaticMesh = NewObject<UStaticMesh>(InParent, InName, Flags | RF_Public);
		for (int32 i = 0; i < RawMesh.FaceMaterialIndices.Num(); ++i) {
			StaticMesh->Materials.Add(UMaterial::GetDefaultMaterial(MD_Surface));
		}
		FStaticMeshSourceModel* StaticMeshSourceModel = new(StaticMesh->SourceModels) FStaticMeshSourceModel();
		StaticMeshSourceModel->RawMeshBulkData->SaveRawMesh(RawMesh);
	}
	return StaticMesh;
}

UVoxel* UVoxFactory::CreateVoxel(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	UVoxel* Voxel = nullptr;
	Voxel = NewObject<UVoxel>(InParent, InName, Flags | RF_Public);
	Voxel->Size = Vox->Size;
	for (FCell cell : Vox->Voxel) {
		Voxel->Voxel.FindOrAdd(cell.ToIntVector()) = cell.I;
	}
	return Voxel;
}

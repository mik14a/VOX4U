// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "VoxelFactory.h"
#include "Editor.h"
#include "Engine.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "Interfaces/IMainFrameModule.h"
#include "MainFrame.h"
#include "RawMesh.h"
#include "SlateCore.h"
#include "Vox.h"
#include "VoxImportOption.h"
#include "VoxOptionWidget.h"
#include "Voxel.h"

UVoxelFactory::UVoxelFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Formats.Add(TEXT("vox;MagicaVoxel"));

	bCreateNew = false;
	bText = false;
	bEditorImport = true;
}

void UVoxelFactory::PostInitProperties()
{
	Super::PostInitProperties();
	ImportOption = NewObject<UVoxImportOption>(this, NAME_None, RF_NoFlags);
}

bool UVoxelFactory::DoesSupportClass(UClass * Class)
{
	return Class == UStaticMesh::StaticClass()
		|| Class == USkeletalMesh::StaticClass()
		|| Class == UVoxel::StaticClass();
}

UClass* UVoxelFactory::ResolveSupportedClass()
{
	UClass* Class = nullptr;
	if (ImportOption->VoxImportType == EVoxImportType::StaticMesh) {
		Class = UStaticMesh::StaticClass();
	} else if (ImportOption->VoxImportType == EVoxImportType::SkeletalMesh) {
		Class = USkeletalMesh::StaticClass();
	} else if (ImportOption->VoxImportType == EVoxImportType::Voxel) {
		Class = UVoxel::StaticClass();
	}
	return Class;
}

UObject* UVoxelFactory::FactoryCreateBinary(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn)
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
	case EVoxImportType::SkeletalMesh:
		Result = CreateSkeletalMesh(InParent, InName, Flags, &Vox);
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

UStaticMesh* UVoxelFactory::CreateStaticMesh(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	UStaticMesh* StaticMesh = nullptr;
	FRawMesh RawMesh;
	if (Vox->CreateRawMesh(RawMesh, ImportOption)) {
		StaticMesh = NewObject<UStaticMesh>(InParent, InName, Flags | RF_Public);
		StaticMesh->Materials.Add(ImportOption->Material ? ImportOption->Material : UMaterial::GetDefaultMaterial(MD_Surface));
		FStaticMeshSourceModel* StaticMeshSourceModel = new(StaticMesh->SourceModels) FStaticMeshSourceModel();
		StaticMeshSourceModel->BuildSettings = ImportOption->BuildSettings;
		StaticMeshSourceModel->RawMeshBulkData->SaveRawMesh(RawMesh);
		TArray<FText> Errors;
		StaticMesh->Build(false, &Errors);
	}
	return StaticMesh;
}

USkeletalMesh* UVoxelFactory::CreateSkeletalMesh(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	USkeletalMesh* SkeletalMesh = nullptr;
	SkeletalMesh = NewObject<USkeletalMesh>(InParent, InName, Flags | RF_Public);
	return SkeletalMesh;
}

UVoxel* UVoxelFactory::CreateVoxel(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	UVoxel* Voxel = nullptr;
	Voxel = NewObject<UVoxel>(InParent, InName, Flags | RF_Public);
	Voxel->Size = Vox->Size;
	for (FCell cell : Vox->Voxel) {
		Voxel->Voxel.FindOrAdd(cell.ToIntVector()) = cell.I;
	}
	Voxel->bXYCenter = ImportOption->bImportXYCenter;
	return Voxel;
}

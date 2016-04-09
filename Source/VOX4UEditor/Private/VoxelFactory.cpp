// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UEditorPrivatePCH.h"
#include "VoxelFactory.h"
#include "Editor.h"
#include "Engine.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "RawMesh.h"
#include "Vox.h"
#include "VoxImportOption.h"
#include "InstancedVoxel.h"
#include "MeshedVoxel.h"

UVoxelFactory::UVoxelFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ImportOption(nullptr)
	, bShowOption(true)
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
		|| Class == UInstancedVoxel::StaticClass()
		|| Class == UMeshedVoxel::StaticClass();
}

UClass* UVoxelFactory::ResolveSupportedClass()
{
	UClass* Class = nullptr;
	if (ImportOption->VoxImportType == EVoxImportType::StaticMesh) {
		Class = UStaticMesh::StaticClass();
	} else if (ImportOption->VoxImportType == EVoxImportType::SkeletalMesh) {
		Class = USkeletalMesh::StaticClass();
	} else if (ImportOption->VoxImportType == EVoxImportType::Voxel) {
		Class = UInstancedVoxel::StaticClass();
	} else if (ImportOption->VoxImportType == EVoxImportType::MeshedVoxel) {
		Class = UMeshedVoxel::StaticClass();
	}
	return Class;
}

UObject* UVoxelFactory::FactoryCreateBinary(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn)
{
	UObject* Result = nullptr;
	FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

	bool ImportAll = false;
	if (!bShowOption || ImportOption->GetImportOption(ImportAll)) {
		bShowOption = !ImportAll;
		FBufferReader Reader((void*)Buffer, BufferEnd - Buffer, false);
		FVox Vox(Reader, ImportOption);
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
		case EVoxImportType::MeshedVoxel:
			Result = CreateMeshedVoxel(InParent, InName, Flags, &Vox);
			break;
		default:
			break;
		}
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
	USkeletalMesh* SkeletalMesh = NewObject<USkeletalMesh>(InParent, InName, Flags | RF_Public);
	return SkeletalMesh;
}

UInstancedVoxel* UVoxelFactory::CreateVoxel(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	UInstancedVoxel* Voxel = NewObject<UInstancedVoxel>(InParent, InName, Flags | RF_Public);
	Voxel->Size = Vox->Size;
	TArray<uint8> Palette;
	for (FCell cell : Vox->Voxel) {
		Palette.AddUnique(cell.I);
	}
	for (uint8 I : Palette) {
		Voxel->Mesh.Add(ImportOption->Mesh);
	}
	for (FCell cell : Vox->Voxel) {
		Voxel->Voxel.Add(FIntVoxel(cell.X, cell.Y, cell.Z, Palette.IndexOfByKey(cell.I)));
		check(INDEX_NONE != Palette.IndexOfByKey(cell.I));
	}
	Voxel->bXYCenter = ImportOption->bImportXYCenter;
	Voxel->CalcCellBounds();
	return Voxel;
}

UMeshedVoxel* UVoxelFactory::CreateMeshedVoxel(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	UMeshedVoxel* MeshedVoxel = NewObject<UMeshedVoxel>(InParent, InName, Flags | RF_Public);
	MeshedVoxel->Size = Vox->Size;
	for (FCell cell : Vox->Voxel) {
		MeshedVoxel->Voxel.Add(FIntVoxel(cell.X, cell.Y, cell.Z, cell.I));
	}
	MeshedVoxel->bXYCenter = ImportOption->bImportXYCenter;
	return MeshedVoxel;
}

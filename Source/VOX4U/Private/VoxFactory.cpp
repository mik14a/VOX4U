// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UPrivatePCH.h"
#include "VoxFactory.h"
#include "Editor.h"
#include "Engine.h"
#include "Engine/StaticMesh.h"
#include "RawMesh.h"
#include "Vox.h"

UVoxFactory::UVoxFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UStaticMesh::StaticClass();
	Formats.Add(TEXT("vox;MagicaVoxel"));

	bCreateNew = false;
	bText = false;
	bEditorImport = true;
}

UObject* UVoxFactory::FactoryCreateBinary(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn)
{
	UStaticMesh* Result = nullptr;

	FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

	FBufferReader Reader((void*)Buffer, BufferEnd - Buffer, false);

	FVox Vox;
	Vox.Import(Reader);

	FRawMesh RawMesh;
	if (Vox.CreateRawMesh(RawMesh)) {

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

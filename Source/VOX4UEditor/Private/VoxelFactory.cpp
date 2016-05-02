// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UEditorPrivatePCH.h"
#include "VoxelFactory.h"
#include "ApexDestructibleAssetImport.h"
#include "Editor.h"
#include "Engine.h"
#include "Engine/DestructibleMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialExpressionConstant4Vector.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "PhysicsEngine/BodySetup.h"
#include "PhysicsEngine/BoxElem.h"
#include "RawMesh.h"
#include "Vox.h"
#include "VoxImportOption.h"
#include "Voxel.h"

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
		|| Class == UDestructibleMesh::StaticClass()
		|| Class == UVoxel::StaticClass();
}

UClass* UVoxelFactory::ResolveSupportedClass()
{
	UClass* Class = nullptr;
	if (ImportOption->VoxImportType == EVoxImportType::StaticMesh) {
		Class = UStaticMesh::StaticClass();
	} else if (ImportOption->VoxImportType == EVoxImportType::SkeletalMesh) {
		Class = USkeletalMesh::StaticClass();
	} else if (ImportOption->VoxImportType == EVoxImportType::DestructibleMesh) {
		Class = UDestructibleMesh::StaticClass();
	} else if (ImportOption->VoxImportType == EVoxImportType::Voxel) {
		Class = UVoxel::StaticClass();
	}
	return Class;
}

UObject* UVoxelFactory::FactoryCreateBinary(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn)
{
	UObject* Result = nullptr;
	FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

	bool bImportAll = true;
	if (!bShowOption || ImportOption->GetImportOption(bImportAll)) {
		bShowOption = !bImportAll;
		FBufferReader Reader((void*)Buffer, BufferEnd - Buffer, false);
		FVox Vox(Reader, ImportOption);
		switch (ImportOption->VoxImportType) {
		case EVoxImportType::StaticMesh:
			Result = CreateStaticMesh(InParent, InName, Flags, &Vox);
			break;
		case EVoxImportType::SkeletalMesh:
			Result = CreateSkeletalMesh(InParent, InName, Flags, &Vox);
			break;
		case EVoxImportType::DestructibleMesh:
			Result = CreateDestructibleMesh(InParent, InName, Flags, &Vox);
			break;
		case EVoxImportType::Voxel:
			Result = CreateVoxel(InParent, InName, Flags, &Vox);
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
	UStaticMesh* StaticMesh = NewObject<UStaticMesh>(InParent, InName, Flags | RF_Public);
	FRawMesh RawMesh;
	Vox->CreateRawMesh(RawMesh, ImportOption);
	UMaterialInterface* Material = CreateMaterial(InParent, InName, Flags, Vox);
	StaticMesh->Materials.Add(Material);
	BuildStaticMesh(StaticMesh, RawMesh);
	return StaticMesh;
}

USkeletalMesh* UVoxelFactory::CreateSkeletalMesh(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	USkeletalMesh* SkeletalMesh = NewObject<USkeletalMesh>(InParent, InName, Flags | RF_Public);
	return SkeletalMesh;
}

UDestructibleMesh* UVoxelFactory::CreateDestructibleMesh(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	UDestructibleMesh* DestructibleMesh = NewObject<UDestructibleMesh>(InParent, InName, Flags | RF_Public);

	FRawMesh RawMesh;
	Vox->CreateRawMesh(RawMesh, ImportOption);
	UMaterialInterface* Material = CreateMaterial(InParent, InName, Flags, Vox);
	UStaticMesh* RootMesh = NewObject<UStaticMesh>();
	RootMesh->Materials.Add(Material);
	BuildStaticMesh(RootMesh, RawMesh);
	DestructibleMesh->SourceStaticMesh = RootMesh;

	TArray<FRawMesh> RawMeshes;
	Vox->CreateRawMeshes(RawMeshes, ImportOption);
	TArray<UStaticMesh*> FractureMeshes;
	for (FRawMesh& RawMesh : RawMeshes) {
		UStaticMesh* FructureMesh = NewObject<UStaticMesh>();
		FructureMesh->Materials.Add(Material);
		BuildStaticMesh(FructureMesh, RawMesh);
		FractureMeshes.Add(FructureMesh);
	}
	DestructibleMesh->SetupChunksFromStaticMeshes(FractureMeshes);
	BuildDestructibleMeshFromFractureSettings(*DestructibleMesh, nullptr);
	DestructibleMesh->SourceStaticMesh = nullptr;

	return DestructibleMesh;
}

UVoxel* UVoxelFactory::CreateVoxel(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	UVoxel* Voxel = NewObject<UVoxel>(InParent, InName, Flags | RF_Public);
	Voxel->Size = Vox->Size;
	TArray<uint8> Palette;
	for (FCell cell : Vox->Voxel) {
		Palette.AddUnique(cell.I);
	}

	UMaterial* Material = NewObject<UMaterial>(InParent, *FString::Printf(TEXT("%s_MT"), *InName.GetPlainNameString()), Flags | RF_Public);
	Material->TwoSided = false;
	Material->SetShadingModel(MSM_DefaultLit);
	UMaterialExpressionVectorParameter* Expression = NewObject<UMaterialExpressionVectorParameter>(Material);
	Expression->ParameterName = TEXT("Color");
	Expression->DefaultValue = FLinearColor::Gray;
	Expression->MaterialExpressionEditorX = -250;
	Material->Expressions.Add(Expression);
	Material->BaseColor.Expression = Expression;
	Material->PostEditChange();

	for (uint8 color : Palette) {
		UMaterialInstanceConstant* MaterialInstance = NewObject<UMaterialInstanceConstant>(InParent, *FString::Printf(TEXT("%s_MI%d"), *InName.GetPlainNameString(), color), Flags | RF_Public);
		MaterialInstance->SetParentEditorOnly(Material);
		FLinearColor LinearColor = FLinearColor::FromSRGBColor(Vox->Palette[color - 1]);
		MaterialInstance->SetVectorParameterValueEditorOnly(TEXT("Color"), LinearColor);

		FRawMesh RawMesh;
		FVox::CreateMesh(RawMesh, ImportOption);
		UStaticMesh* StaticMesh = NewObject<UStaticMesh>(InParent, *FString::Printf(TEXT("%s_SM%d"), *InName.GetPlainNameString(), color), Flags | RF_Public);
		StaticMesh->Materials.Add(MaterialInstance);
		BuildStaticMesh(StaticMesh, RawMesh);

		const FVector& Scale = ImportOption->GetBuildSettings().BuildScale3D;
		FKBoxElem BoxElem(Scale.X, Scale.Y, Scale.Z);
		StaticMesh->BodySetup->AggGeom.BoxElems.Add(BoxElem);

		Voxel->Mesh.Add(StaticMesh);
	}
	for (FCell cell : Vox->Voxel) {
		Voxel->Voxel.Add(FIntVoxel(cell.X, cell.Y, cell.Z, Palette.IndexOfByKey(cell.I)));
		check(INDEX_NONE != Palette.IndexOfByKey(cell.I));
	}
	Voxel->bXYCenter = ImportOption->bImportXYCenter;
	Voxel->CalcCellBounds();
	return Voxel;
}

UStaticMesh* UVoxelFactory::BuildStaticMesh(UStaticMesh* OutStaticMesh, FRawMesh& RawMesh) const
{
	check(OutStaticMesh);
	FStaticMeshSourceModel* StaticMeshSourceModel = new(OutStaticMesh->SourceModels) FStaticMeshSourceModel();
	StaticMeshSourceModel->BuildSettings = ImportOption->GetBuildSettings();
	StaticMeshSourceModel->RawMeshBulkData->SaveRawMesh(RawMesh);
	TArray<FText> Errors;
	OutStaticMesh->Build(false, &Errors);
	return OutStaticMesh;
}

UMaterialInterface* UVoxelFactory::CreateMaterial(UObject* InParent, FName &InName, EObjectFlags Flags, const FVox* Vox) const
{
	UMaterial* Material = NewObject<UMaterial>(InParent, *FString::Printf(TEXT("%s_MT"), *InName.GetPlainNameString()), Flags | RF_Public);
	UTexture2D* Texture = NewObject<UTexture2D>(InParent, *FString::Printf(TEXT("%s_TX"), *InName.GetPlainNameString()), Flags | RF_Public);
	if (Vox->CreateTexture(Texture, ImportOption)) {
		Material->TwoSided = false;
		Material->SetShadingModel(MSM_DefaultLit);
		UMaterialExpressionTextureSample* Expression = NewObject<UMaterialExpressionTextureSample>(Material);
		Material->Expressions.Add(Expression);
		Material->BaseColor.Expression = Expression;
		Expression->Texture = Texture;
		Material->PostEditChange();
	}
	return Material;
}

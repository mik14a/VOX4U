// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#include "VoxelFactory.h"
#include <ApexDestructibleAssetImport.h>
#include <DestructibleMesh.h>
#include <Editor.h>
#include <EditorFramework/AssetImportData.h>
#include <Engine/SkeletalMesh.h>
#include <Engine/StaticMesh.h>
#include <HAL/FileManager.h>
#include <Materials/MaterialExpressionVectorParameter.h>
#include <Materials/MaterialInstanceConstant.h>
#include <PhysicsEngine/BodySetup.h>
#include <PhysicsEngine/BoxElem.h>
#include <RawMesh.h>
#include "VOX.h"
#include "VoxAssetImportData.h"
#include "VoxImportOption.h"
#include "Voxel.h"

DEFINE_LOG_CATEGORY_STATIC(LogVoxelFactory, Log, All)

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
		FVox Vox(GetCurrentFilename(), Reader, ImportOption);
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

bool UVoxelFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	UStaticMesh* StaticMesh = Cast<UStaticMesh>(Obj);
	USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(Obj);
	UDestructibleMesh* DestructibleMesh = Cast<UDestructibleMesh>(Obj);
	UVoxel* Voxel = Cast<UVoxel>(Obj);

	const auto& AssetImportData = StaticMesh != nullptr ? StaticMesh->AssetImportData
		: SkeletalMesh != nullptr ? SkeletalMesh->AssetImportData
		: DestructibleMesh != nullptr ? DestructibleMesh->AssetImportData
		: Voxel != nullptr ? Voxel->AssetImportData
		: nullptr;
	if (AssetImportData != nullptr) {
		const auto& SourcePath = AssetImportData->GetFirstFilename();
		FString Path, Filename, Extension;
		FPaths::Split(SourcePath, Path, Filename, Extension);
		if (Extension.Compare("vox", ESearchCase::IgnoreCase) == 0) {
			AssetImportData->ExtractFilenames(OutFilenames);
			return true;
		}
	}
	return false;
}

void UVoxelFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	UStaticMesh* StaticMesh = Cast<UStaticMesh>(Obj);
	USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(Obj);
	UDestructibleMesh* DestructibleMesh = Cast<UDestructibleMesh>(Obj);
	UVoxel* Voxel = Cast<UVoxel>(Obj);

	const auto& AssetImportData = StaticMesh ? StaticMesh->AssetImportData
		: SkeletalMesh ? SkeletalMesh->AssetImportData
		: DestructibleMesh ? DestructibleMesh->AssetImportData
		: Voxel ? Voxel->AssetImportData
		: nullptr;
	if (AssetImportData && ensure(NewReimportPaths.Num() == 1)) {
		AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
	}
}

EReimportResult::Type UVoxelFactory::Reimport(UObject* Obj)
{
	UStaticMesh* StaticMesh = Cast<UStaticMesh>(Obj);
	USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(Obj);
	UDestructibleMesh* DestructibleMesh = Cast<UDestructibleMesh>(Obj);
	UVoxel* Voxel = Cast<UVoxel>(Obj);

	const auto& AssetImportData = StaticMesh ? Cast<UVoxAssetImportData>(StaticMesh->AssetImportData)
		: SkeletalMesh ? Cast<UVoxAssetImportData>(SkeletalMesh->AssetImportData)
		: DestructibleMesh ? Cast<UVoxAssetImportData>(DestructibleMesh->AssetImportData)
		: Voxel ? Cast<UVoxAssetImportData>(Voxel->AssetImportData)
		: nullptr;
	if (!AssetImportData) {
		return EReimportResult::Failed;
	}

	const auto& Filename = AssetImportData->GetFirstFilename();
	if (!Filename.Len() || IFileManager::Get().FileSize(*Filename) == INDEX_NONE) {
		return EReimportResult::Failed;
	}

	auto Result = EReimportResult::Failed;
	auto OutCanceled = false;
	AssetImportData->ToVoxImportOption(*ImportOption);
	bShowOption = false;
	if (ImportObject(Obj->GetClass(), Obj->GetOuter(), *Obj->GetName(), RF_Public | RF_Standalone, Filename, nullptr, OutCanceled) != nullptr) {
		UE_LOG(LogVoxelFactory, Verbose, TEXT("Reimport successfully."));
		AssetImportData->Update(Filename);
		if (Obj->GetOuter()) {
			Obj->GetOuter()->MarkPackageDirty();
		} else {
			Obj->MarkPackageDirty();
		}
		Result = EReimportResult::Succeeded;
	} else {
		if (OutCanceled) {
			UE_LOG(LogVoxelFactory, Warning, TEXT("Reimport canceled."));
		} else {
			UE_LOG(LogVoxelFactory, Warning, TEXT("Reimport failed."));
		}
		Result = EReimportResult::Failed;
	}
	return Result;
}

UStaticMesh* UVoxelFactory::CreateStaticMesh(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	UStaticMesh* StaticMesh = NewObject<UStaticMesh>(InParent, InName, Flags | RF_Public);
	if (!StaticMesh->AssetImportData || !StaticMesh->AssetImportData->IsA<UVoxAssetImportData>()) {
		auto AssetImportData = NewObject<UVoxAssetImportData>(StaticMesh);
		AssetImportData->FromVoxImportOption(*ImportOption);
		StaticMesh->AssetImportData = AssetImportData;
	}

	FRawMesh RawMesh;
	Vox->CreateOptimizedRawMesh(RawMesh, ImportOption);
	UMaterialInterface* Material = CreateMaterial(InParent, InName, Flags, Vox);
	StaticMesh->StaticMaterials.Add(FStaticMaterial(Material));
	BuildStaticMesh(StaticMesh, RawMesh);
	StaticMesh->AssetImportData->Update(Vox->Filename);
	return StaticMesh;
}

USkeletalMesh* UVoxelFactory::CreateSkeletalMesh(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	USkeletalMesh* SkeletalMesh = NewObject<USkeletalMesh>(InParent, InName, Flags | RF_Public);
	if (!SkeletalMesh->AssetImportData || !SkeletalMesh->AssetImportData->IsA<UVoxAssetImportData>()) {
		auto AssetImportData = NewObject<UVoxAssetImportData>(SkeletalMesh);
		AssetImportData->FromVoxImportOption(*ImportOption);
		SkeletalMesh->AssetImportData = AssetImportData;
	}

	SkeletalMesh->AssetImportData->Update(Vox->Filename);
	return SkeletalMesh;
}

/**
 * CreateDestructibleMesh
 * @param InParent Import package
 * @param InName Package name
 * @param Flags Import flags
 * @param Vox Voxel file data
 */
UDestructibleMesh* UVoxelFactory::CreateDestructibleMesh(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	UDestructibleMesh* DestructibleMesh = NewObject<UDestructibleMesh>(InParent, InName, Flags | RF_Public);
	if (!DestructibleMesh->AssetImportData || !DestructibleMesh->AssetImportData->IsA<UVoxAssetImportData>()) {
		auto AssetImportData = NewObject<UVoxAssetImportData>(DestructibleMesh);
		AssetImportData->FromVoxImportOption(*ImportOption);
		DestructibleMesh->AssetImportData = AssetImportData;
	}

	FRawMesh RawMesh;
	Vox->CreateOptimizedRawMesh(RawMesh, ImportOption);
	UMaterialInterface* Material = CreateMaterial(InParent, InName, Flags, Vox);
	UStaticMesh* RootMesh = NewObject<UStaticMesh>();
	RootMesh->StaticMaterials.Add(FStaticMaterial(Material));
	BuildStaticMesh(RootMesh, RawMesh);
	DestructibleMesh->SourceStaticMesh = RootMesh;

	TArray<FRawMesh> RawMeshes;
	Vox->CreateRawMeshes(RawMeshes, ImportOption);
	TArray<UStaticMesh*> FractureMeshes;
	for (FRawMesh& RawMesh : RawMeshes) {
		UStaticMesh* FructureMesh = NewObject<UStaticMesh>();
		FructureMesh->StaticMaterials.Add(FStaticMaterial(Material));
		BuildStaticMesh(FructureMesh, RawMesh);
		FractureMeshes.Add(FructureMesh);
	}
	DestructibleMesh->SetupChunksFromStaticMeshes(FractureMeshes);
	BuildDestructibleMeshFromFractureSettings(*DestructibleMesh, nullptr);
	DestructibleMesh->SourceStaticMesh = nullptr;
	DestructibleMesh->AssetImportData->Update(Vox->Filename);

	return DestructibleMesh;
}

UVoxel* UVoxelFactory::CreateVoxel(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const
{
	UVoxel* Voxel = NewObject<UVoxel>(InParent, InName, Flags | RF_Public);
	if (!Voxel->AssetImportData || !Voxel->AssetImportData->IsA<UVoxAssetImportData>()) {
		auto AssetImportData = NewObject<UVoxAssetImportData>(Voxel);
		AssetImportData->FromVoxImportOption(*ImportOption);
		Voxel->AssetImportData = AssetImportData;
	}
	Voxel->Size = Vox->Size;
	TArray<uint8> Palette;
	for (const auto& cell : Vox->Voxel) {
		Palette.AddUnique(cell.Value);
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
		StaticMesh->StaticMaterials.Add(FStaticMaterial(MaterialInstance));
		BuildStaticMesh(StaticMesh, RawMesh);

		const FVector& Scale = ImportOption->GetBuildSettings().BuildScale3D;
		FKBoxElem BoxElem(Scale.X, Scale.Y, Scale.Z);
		StaticMesh->BodySetup->AggGeom.BoxElems.Add(BoxElem);

		Voxel->Mesh.Add(StaticMesh);
	}
	for (const auto& cell : Vox->Voxel) {
		Voxel->Voxel.Add(cell.Key, Palette.IndexOfByKey(cell.Value));
		check(INDEX_NONE != Palette.IndexOfByKey(cell.Value));
	}
	Voxel->bXYCenter = ImportOption->bImportXYCenter;
	Voxel->CalcCellBounds();
	Voxel->AssetImportData->Update(Vox->Filename);
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

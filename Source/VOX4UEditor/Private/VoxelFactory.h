// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <EditorReimportHandler.h>
#include <Factories/Factory.h>
#include <RawMesh.h>
#include "VoxelFactory.generated.h"

struct FVox;
class UDestructibleMesh;
class UMaterialInterface;
class USkeletalMesh;
class UStaticMesh;
class UVoxImportOption;
class UVoxel;

/**
 * Factory
 */
UCLASS()
class UVoxelFactory : public UFactory, public FReimportHandler
{
	GENERATED_BODY()

public:

	UVoxelFactory(const FObjectInitializer& ObjectInitializer);

	void PostInitProperties() override;

	virtual bool DoesSupportClass(UClass * Class) override;

	virtual UClass* ResolveSupportedClass() override;

	virtual UObject* FactoryCreateBinary(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn) override;

	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;

	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;

	virtual EReimportResult::Type Reimport(UObject* Obj) override;

private:

	UStaticMesh* CreateStaticMesh(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const;

	USkeletalMesh* CreateSkeletalMesh(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const;

	UDestructibleMesh* CreateDestructibleMesh(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const;

	UVoxel* CreateVoxel(UObject* InParent, FName InName, EObjectFlags Flags, const FVox* Vox) const;

	UStaticMesh* BuildStaticMesh(UStaticMesh* OutStaticMesh, FRawMesh& RawMesh) const;

	UMaterialInterface* CreateMaterial(UObject* InParent, FName &InName, EObjectFlags Flags, const FVox* Vox) const;

protected:

	UPROPERTY()
	UVoxImportOption* ImportOption;
	bool bShowOption;
};

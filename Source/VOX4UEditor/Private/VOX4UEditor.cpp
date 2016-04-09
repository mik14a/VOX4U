// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#include "VOX4UEditorPrivatePCH.h"

#define LOCTEXT_NAMESPACE "FVOX4UEditorModule"

void FVOX4UEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FVOX4UEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVOX4UEditorModule, VOX4UEditor)

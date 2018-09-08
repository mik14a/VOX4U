// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#include "VOX4U.h"

#define LOCTEXT_NAMESPACE "FVOX4UModule"

void FVOX4UModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FVOX4UModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVOX4UModule, VOX4U)

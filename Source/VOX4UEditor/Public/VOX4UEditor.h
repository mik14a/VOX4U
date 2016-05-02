// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

class FVOX4UEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

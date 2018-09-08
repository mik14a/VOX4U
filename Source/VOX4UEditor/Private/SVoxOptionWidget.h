// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Widgets/SCompoundWidget.h>
#include <Widgets/SWindow.h>

class UVoxImportOption;

/**
 * Option widget
 */
class SVoxOptionWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SVoxOptionWidget)
		: _Window(nullptr)
		, _ImportOption(nullptr) { }
		SLATE_ARGUMENT(TSharedPtr<SWindow>, Window)
		SLATE_ARGUMENT(UVoxImportOption*, ImportOption)
	SLATE_END_ARGS()

public:

	SVoxOptionWidget();

	void Construct(const FArguments& InArgs);

	FReply OnImportAll();

	FReply OnImport();

	FReply OnCancel();

	bool ShouldImportAll() const {
		return bShouldImportAll;
	}

	bool ShouldImport() const {
		return bShouldImport;
	}

private:

	TWeakPtr<SWindow> Window;
	UVoxImportOption* ImportOption;
	bool bShouldImportAll;
	bool bShouldImport;
};

// Copyright 2016 mik14a / Admix Network. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "DeclarativeSyntaxSupport.h"
#include "Reply.h"

class UVoxImportOption;

/**
 *
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

private:

	TWeakPtr<SWindow> Window;
	UVoxImportOption* ImportOption;
	bool bShouldImportAll;
	bool bShouldImport;
};

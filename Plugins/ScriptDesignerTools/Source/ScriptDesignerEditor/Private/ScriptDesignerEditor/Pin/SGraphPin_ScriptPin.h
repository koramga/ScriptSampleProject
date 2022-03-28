// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphPin_BaseScriptPin.h"

/**
 * 
 */
class SGraphPin_ScriptPin : public SGraphPin_BaseScriptPin
{
public:
	SLATE_BEGIN_ARGS(SGraphPin_ScriptPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected:
	virtual FSlateColor GetPinColor() const override;
	virtual TSharedRef<SWidget> GetDefaultValueWidget() override;
	const FSlateBrush* GetPinBorder() const;
	virtual TSharedRef<FDragDropOperation> SpawnPinDragEvent(const TSharedRef<SGraphPanel>& InGraphPanel, const TArray<TSharedRef<SGraphPin>>& InStartingPins) override;
};

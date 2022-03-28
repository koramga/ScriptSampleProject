// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

/**
 * 
 */
class SCRIPTDESIGNEREDITOR_API SGraphNode_BaseScriptNode : public SGraphNode
{
public :
	SLATE_BEGIN_ARGS(SGraphNode_BaseScriptNode) {}
	SLATE_END_ARGS()

	virtual FSlateColor GetBorderBackgroundColor() const;
	virtual FSlateColor GetBackgroundColor() const;

	virtual EVisibility GetDragOverMarkerVisibility() const;

	virtual const FSlateBrush* GetNameIcon() const;

	void OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo);
	virtual bool IsNameReadOnly() const override;
};

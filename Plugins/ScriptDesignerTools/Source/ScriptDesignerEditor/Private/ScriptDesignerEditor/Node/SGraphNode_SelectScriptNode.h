// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode_BaseScriptNode.h"

class UEdGraphNode_SelectScriptNode;

/**
 * 
 */
class SCRIPTDESIGNEREDITOR_API SGraphNode_SelectScriptNode : public SGraphNode_BaseScriptNode
{
public :
	SLATE_BEGIN_ARGS(SGraphNode_SelectScriptNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode_SelectScriptNode* InNode);
	virtual void UpdateGraphNode() override;

protected:
	virtual void SetDefaultTitleAreaWidget(TSharedRef<SOverlay> DefaultTitleAreaWidget) override;
};

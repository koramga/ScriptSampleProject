// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode_BaseScriptNode.h"

class UEdGraphNode_ScriptNode;

/**
 * 
 */
class SCRIPTDESIGNEREDITOR_API SGraphNode_ScriptNode : public SGraphNode_BaseScriptNode
{
public :
	SLATE_BEGIN_ARGS(SGraphNode_ScriptNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode_ScriptNode* InNode);

	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
};

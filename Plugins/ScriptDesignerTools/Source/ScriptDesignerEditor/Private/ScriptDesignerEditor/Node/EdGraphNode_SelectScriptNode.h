// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScriptDesignerEditor/Node/EdGraphNode_BaseScriptNode.h"
#include "EdGraphNode_SelectScriptNode.generated.h"

/**
 * 
 */
UCLASS()
class SCRIPTDESIGNEREDITOR_API UEdGraphNode_SelectScriptNode : public UEdGraphNode_BaseScriptNode
{
	GENERATED_BODY()

protected:
	virtual void AllocateDefaultPins() override;
};

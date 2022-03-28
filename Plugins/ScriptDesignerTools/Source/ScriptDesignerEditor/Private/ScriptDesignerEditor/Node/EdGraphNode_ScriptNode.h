// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScriptDesignerEditor/Node/EdGraphNode_BaseScriptNode.h"
#include "ScriptDesignerEditor/EdGraph_ScriptGraph.h"
#include "ScriptGraphNode.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraphNode_ScriptNode.generated.h"

/**
 * 
 */
UCLASS()
class SCRIPTDESIGNEREDITOR_API UEdGraphNode_ScriptNode : public UEdGraphNode_BaseScriptNode
{
	GENERATED_BODY()

public :
	UEdGraphNode_ScriptNode();
	virtual ~UEdGraphNode_ScriptNode();	
};

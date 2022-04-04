// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EdGraph/EdGraph.h"
#include "ScriptDesignerEditor/EdGraph_ScriptGraph.h"
#include "ScriptDesignerEditor/Node/EdGraphNode_BaseScriptNode.h"
#include "ScriptDesignerEditor/Edge/EdNode_ScriptGraphEdge.h"
#include "ScriptDesignerEditor/Settings_ScriptGraphEditor.h"
#include "AutoLayoutStrategy.generated.h"

/**
 * 
 */
UCLASS(abstract)
class SCRIPTDESIGNEREDITOR_API UAutoLayoutStrategy : public UObject
{
	GENERATED_BODY()
	
public :
	UAutoLayoutStrategy();
	virtual ~UAutoLayoutStrategy();

	virtual void Layout(UEdGraph* G) {};

	class UScriptGraphEditorSettings* Settings;

protected :
	int32 GetNodeWidth(UEdNode_ScriptGraphNode* EdNode);
	int32 GetNodeHeight(UEdNode_ScriptGraphNode* EdNode);
	FBox2D GetNodeBound(UEdGraphNode* EdNode);
	FBox2D GetActualBounds(UScriptGraphNode* RootNode);
	virtual void RandomLayoutOnTree(UScriptGraphNode* RootNode, const FBox2D& Bound);

protected :
	UScriptGraph* Graph;
	UEdGraph_ScriptGraph* EdGraph;
	int32 MaxIteration;
	int32 OptimalDistance;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptGraphNodeFactory.h"
#include <EdGraph/EdGraphNode.h>
#include "ScriptDesignerEditor/Edge/SEdNode_ScriptGraphEdge.h"
#include "ScriptDesignerEditor/Edge/EdNode_ScriptGraphEdge.h"
#include "ScriptDesignerEditor/Node/SEdNode_ScriptGraphNode.h"
#include "ScriptDesignerEditor/Node/EdNode_ScriptGraphNode.h"

TSharedPtr<class SGraphNode> FGraphPanelNodeFactory_ScriptGraph::CreateNode(UEdGraphNode* Node) const
{
	if (UEdNode_ScriptGraphNode* EdNode_ScriptNode = Cast<UEdNode_ScriptGraphNode>(Node))
	{
		return SNew(SEdNode_ScriptGraphNode, EdNode_ScriptNode);
	}
	else if (UEdNode_ScriptGraphEdge* EdNode_Edge = Cast <UEdNode_ScriptGraphEdge>(Node))
	{
		return SNew(SEdNode_ScriptGraphEdge, EdNode_Edge);
	}

	return nullptr;
}

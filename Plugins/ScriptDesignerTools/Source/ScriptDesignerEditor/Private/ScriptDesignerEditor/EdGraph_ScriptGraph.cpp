#include "ScriptDesignerEditor/EdGraph_ScriptGraph.h"
#include "ScriptDesignerEditor/EdNode_ScriptGraphNode.h"
#include "ScriptGraphEditorPCH.h"

UEdGraph_ScriptGraph::UEdGraph_ScriptGraph()
{
}

UEdGraph_ScriptGraph::~UEdGraph_ScriptGraph()
{
}

void UEdGraph_ScriptGraph::RebuildGenericGraph()
{
	LOG_INFO(TEXT("UScriptGraphEdGraph::RebuildScriptGraph has been called"));

	UScriptGraph* Graph = GetScriptGraph();

	Clear();

	for(int i = 0; i < Nodes.Num(); ++i)
	{
		if(UEdNode_ScriptGraphNode* EdNode = Cast<UEdNode_ScriptGraphNode>(Nodes[i]))
		{
			//if(EdNode->)
		}
	}
}

UScriptGraph* UEdGraph_ScriptGraph::GetScriptGraph() const
{
	return nullptr;
}

bool UEdGraph_ScriptGraph::Modify(bool bAlwaysMarkDirty)
{
	return Super::Modify(bAlwaysMarkDirty);
}

void UEdGraph_ScriptGraph::PostEditUndo()
{
	Super::PostEditUndo();
}

void UEdGraph_ScriptGraph::Clear()
{
}

void UEdGraph_ScriptGraph::SortNodes(UScriptGraphNode* RootNode)
{
}

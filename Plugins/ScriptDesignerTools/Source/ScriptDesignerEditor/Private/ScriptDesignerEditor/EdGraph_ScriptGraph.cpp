#include "ScriptDesignerEditor/EdGraph_ScriptGraph.h"
#include "ScriptDesignerEditor/Node/EdGraphNode_BaseScriptNode.h"
#include "ScriptDesignerEditor/Edge/EdNode_ScriptGraphEdge.h"
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
		if(UEdGraphNode_BaseScriptNode* EdNode = Cast<UEdGraphNode_BaseScriptNode>(Nodes[i]))
		{
			if(EdNode->GetScriptGraphNode() == nullptr)
				continue;

			UScriptGraphNode* ScriptGraphNode = EdNode->GetScriptGraphNode();

			NodeMap.Add(ScriptGraphNode, EdNode);

			Graph->AddNode(ScriptGraphNode);

			for (int PinIdx = 0; PinIdx < EdNode->Pins.Num(); ++PinIdx)
			{
				UEdGraphPin* Pin = EdNode->Pins[PinIdx];

				if (Pin->Direction != EEdGraphPinDirection::EGPD_Output)
					continue;

				for (int LinkToIdx = 0; LinkToIdx < Pin->LinkedTo.Num(); ++LinkToIdx)
				{
					UScriptGraphNode* ChildNode = nullptr;
					if (UEdGraphNode_BaseScriptNode* EdNode_Child = Cast<UEdGraphNode_BaseScriptNode>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						ChildNode = EdNode_Child->GetScriptGraphNode();
					}
					else if (UEdNode_ScriptGraphEdge* EdNode_Edge = Cast<UEdNode_ScriptGraphEdge>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						UEdGraphNode_BaseScriptNode* Child = EdNode_Edge->GetEndNode();
						if (Child != nullptr)
						{
							ChildNode = Child->GetScriptGraphNode();
						}
					}

					if (ChildNode != nullptr)
					{
						ScriptGraphNode->AddChildNode(ChildNode);
						ChildNode->AddParentNode(ScriptGraphNode);
					}
					else
					{
						LOG_ERROR(TEXT("UEdGraph_ScriptGraph::RebuildScriptGraph can't find child node"));
					}
				}
			}
		}
		else if (UEdNode_ScriptGraphEdge* EdgeNode = Cast <UEdNode_ScriptGraphEdge>(Nodes[i]))
		{
			UEdGraphNode_BaseScriptNode* StartNode = EdgeNode->GetStartNode();
			UEdGraphNode_BaseScriptNode* EndNode = EdgeNode->GetEndNode();
			UScriptGraphEdge* Edge = EdgeNode->GetEdge();

			if (StartNode == nullptr || EndNode == nullptr || Edge == nullptr)
			{
				LOG_ERROR(TEXT("UEdGraph_ScriptGraph::RebuildScriptGraph add edge failed"));
				continue;
			}

			EdgeMap.Add(Edge, EdgeNode);

			Edge->SetGraph(Graph);
			Edge->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
			Edge->SetStartNode(StartNode->GetScriptGraphNode());
			Edge->SetEndNode(EndNode->GetScriptGraphNode());
			Edge->GetStartNode()->AddEdge(Edge->GetEndNode(), Edge);
		}
	}

	const TArray<UScriptGraphNode*>& GraphAllNodes = Graph->GetAllNodes();

	for (int i = 0; i < GraphAllNodes.Num(); ++i)
	{
		UScriptGraphNode* Node = GraphAllNodes[i];
		if (Node->IsEmptyParentNode())
		{
			Graph->AddRootNode(Node);
			SortNodes(Node);
		}

		Node->SetGraph(Graph);
		Node->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
	}

	TArray<UScriptGraphNode*>& RootNodes = Graph->GetRootNodes();

	RootNodes.Sort([&](const UScriptGraphNode& L, const UScriptGraphNode& R)
	{
		UEdGraphNode_BaseScriptNode* EdNode_LNode = NodeMap[&L];
		UEdGraphNode_BaseScriptNode* EdNode_RNode = NodeMap[&R];
		return EdNode_LNode->NodePosX < EdNode_RNode->NodePosX;
	});
}

UScriptGraph* UEdGraph_ScriptGraph::GetScriptGraph() const
{
	return CastChecked<UScriptGraph>(GetOuter());
}

bool UEdGraph_ScriptGraph::Modify(bool bAlwaysMarkDirty)
{
	bool Rtn = Super::Modify(bAlwaysMarkDirty);

	GetScriptGraph()->Modify();

	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		Nodes[i]->Modify();
	}

	return Rtn;
}

void UEdGraph_ScriptGraph::PostEditUndo()
{
	Super::PostEditUndo();

	NotifyGraphChanged();
}

UEdGraphNode_BaseScriptNode* UEdGraph_ScriptGraph::FindNode(UScriptGraphNode* Key)
{
	return NodeMap[Key];
}

UEdNode_ScriptGraphEdge* UEdGraph_ScriptGraph::FindEdge(UScriptGraphEdge* Key)
{
	return EdgeMap[Key];
}

void UEdGraph_ScriptGraph::Clear()
{
	UScriptGraph* Graph = GetScriptGraph();

	Graph->ClearGraph();
	NodeMap.Reset();
	EdgeMap.Reset();

	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (UEdGraphNode_BaseScriptNode* EdNode = Cast<UEdGraphNode_BaseScriptNode>(Nodes[i]))
		{
			UScriptGraphNode* ScriptGraphNode = EdNode->GetScriptGraphNode();
			if (ScriptGraphNode)
			{
				ScriptGraphNode->ClearParentNodes();
				ScriptGraphNode->ClearChildNodes();
				ScriptGraphNode->ClearEdges();
			}
		}
	}
}

void UEdGraph_ScriptGraph::SortNodes(UScriptGraphNode* RootNode)
{
	int Level = 0;
	TArray<UScriptGraphNode*> CurrLevelNodes = { RootNode };
	TArray<UScriptGraphNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		int32 LevelWidge = 0;

		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UScriptGraphNode* Node = CurrLevelNodes[i];

			auto Comp = [&](const UScriptGraphNode& L, const UScriptGraphNode& R)
			{
				UEdGraphNode_BaseScriptNode* EdNode_LNode = NodeMap[&L];
				UEdGraphNode_BaseScriptNode* EdNode_RNode = NodeMap[&R];
				return EdNode_LNode->NodePosX < EdNode_RNode->NodePosX;
			};

			TArray<UScriptGraphNode*>& ChildrenNodes = Node->GetChildrenNodes();

			ChildrenNodes.Sort(Comp);
			Node->GetParentNodes().Sort(Comp);

			for (int j = 0; j < ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(ChildrenNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptGraph.h"
#include "ScriptDesignerRuntimePCH.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "ScriptGraph"

UScriptGraph::UScriptGraph()
{
	NodeType = UScriptGraphNode::StaticClass();
	EdgeType = UScriptGraphEdge::StaticClass();

	bIsEdgeEnabled = true;

#if WITH_EDITORONLY_DATA

	EdGraph = nullptr;
	
	bCanRenameNode = true;
	
#endif
}

UScriptGraph::~UScriptGraph()
{
	
}

FString UScriptGraph::GetName() const
{
	return Name;
}

void UScriptGraph::SetName(const FString& InName)
{
	Name = InName;
}

bool UScriptGraph::IsValidNodeTypeClass() const
{
	if(IsValid(NodeType))
	{
		return true;
	}

	return false;
}

TSubclassOf<UScriptGraphNode>& UScriptGraph::GetNodeTypeClass()
{
	return NodeType;
}

bool UScriptGraph::IsValidEdgeTypeClass() const
{
	if(IsValid(EdgeType))
	{
		return true;
	}

	return false;
}

TSubclassOf<UScriptGraphEdge>& UScriptGraph::GetEdgeTypeClass()
{
	return EdgeType;
}

bool UScriptGraph::IsEdgeEnabled() const
{
	return bIsEdgeEnabled;
}

void UScriptGraph::SetEdgeEnabled(bool InbIsEdgeEnabled)
{
	bIsEdgeEnabled = InbIsEdgeEnabled;
}

const TArray<UScriptGraphNode*>& UScriptGraph::GetAllNodes() const
{
	return AllNodes;
}

void UScriptGraph::AddNode(UScriptGraphNode* ScriptGraphNode)
{
	AllNodes.Add(ScriptGraphNode);
}

void UScriptGraph::AddRootNode(UScriptGraphNode* InNode)
{
	RootNodes.Add(InNode);
}

TArray<UScriptGraphNode*>& UScriptGraph::GetRootNodes()
{
	return RootNodes;
}

void UScriptGraph::Print(bool ToConsole, bool ToScreen)
{
	int32 Level = 0;
	TArray<UScriptGraphNode*> CurrLevelNodes = RootNodes;
	TArray<UScriptGraphNode*> NextLevelNodes;

	while(CurrLevelNodes.Num() != 0)
	{
		for(int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UScriptGraphNode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			FString Message = FString::Printf(TEXT("%s, Level %d"), *Node->GetDescription().ToString(), Level);

			if(ToConsole)
			{
				LOG_INFO(TEXT("%s"), *Message);
			}

			if(ToScreen && GEngine != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, Message);
			}

			const TArray<UScriptGraphNode*>& ChildrenNodeFromNode = Node->GetChildrenNodes();
			
			for(UScriptGraphNode* ChildNode : ChildrenNodeFromNode)
			{
				NextLevelNodes.Add(ChildNode);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}
}

int UScriptGraph::GetLevelNum() const
{
	int32 Level = 0;
	TArray<UScriptGraphNode*> CurrLevelNodes = RootNodes;
	TArray<UScriptGraphNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UScriptGraphNode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			const TArray<UScriptGraphNode*>& ChildrenNodeFromNode = Node->GetChildrenNodes();
			
			for (UScriptGraphNode* ChildNode : ChildrenNodeFromNode)
			{
				NextLevelNodes.Add(ChildNode);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}
	
	return Level;
}

void UScriptGraph::GetNodesByLevel(int Level, TArray<UScriptGraphNode*>& Nodes)
{
	int CurrLEvel = 0;
	TArray<UScriptGraphNode*> NextLevelNodes;

	Nodes = RootNodes;

	while (Nodes.Num() != 0)
	{
		if (CurrLEvel == Level)
			break;

		for (int i = 0; i < Nodes.Num(); ++i)
		{
			UScriptGraphNode* Node = Nodes[i];
			check(Node != nullptr);

			const TArray<UScriptGraphNode*>& ChildrenNodeFromNode = Node->GetChildrenNodes();

			for (UScriptGraphNode* ChildNode : ChildrenNodeFromNode)
			{
				NextLevelNodes.Add(ChildNode);
			}
		}

		Nodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++CurrLEvel;
	}
}

void UScriptGraph::ClearGraph()
{
	for (int i = 0; i < AllNodes.Num(); ++i)
	{
		UScriptGraphNode* Node = AllNodes[i];
		if (nullptr != Node)
		{
			Node->Clear();			
		}
	}

	AllNodes.Empty();
	RootNodes.Empty();
}

#undef LOCTEXT_NAMESPACE
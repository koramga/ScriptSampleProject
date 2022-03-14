// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptGraphNode.h"
#include "ScriptGraph.h"

#define LOCTEXT_NAMESPACE "ScriptGraphNode"

UScriptGraphNode::UScriptGraphNode()
{
}

UScriptGraphNode::~UScriptGraphNode()
{
}

TArray<UScriptGraphNode*>& UScriptGraphNode::GetChildrenNodes()
{
	return ChildrenNodes;
}

TArray<UScriptGraphNode*>& UScriptGraphNode::GetParentNodes()
{
	return ParentNodes;
}

void UScriptGraphNode::Clear()
{
	ParentNodes.Empty();
	ChildrenNodes.Empty();
	Edges.Empty();
}

void UScriptGraphNode::AddChildNode(UScriptGraphNode* ChildNode)
{
	ChildrenNodes.Add(ChildNode);
}

void UScriptGraphNode::RemoveChildNode(UScriptGraphNode* ChildNode)
{
	ChildrenNodes.Remove(ChildNode);
}

void UScriptGraphNode::ClearChildNodes()
{
	ChildrenNodes.Reset();
}

bool UScriptGraphNode::IsEmptyChildNode() const
{
	if (ParentNodes.Num() == 0)
	{
		return true;
	}

	return false;
}

void UScriptGraphNode::AddParentNode(UScriptGraphNode* ParentNode)
{
	ParentNodes.Add(ParentNode);
}

void UScriptGraphNode::RemoveParentNode(UScriptGraphNode* ParentNode)
{
	ParentNodes.Remove(ParentNode);
}

void UScriptGraphNode::ClearParentNodes()
{
	ParentNodes.Reset();
}

bool UScriptGraphNode::IsEmptyParentNode() const
{
	if (ParentNodes.Num() == 0)
	{
		return true;
	}

	return false;
}

void UScriptGraphNode::AddEdge(UScriptGraphNode* InNode, UScriptGraphEdge* InEdge)
{
	Edges.Add(InNode, InEdge);
}

void UScriptGraphNode::SetGraph(UScriptGraph* InGraph)
{
	Graph = InGraph;
}

void UScriptGraphNode::ClearEdges()
{
	Edges.Reset();
}

UScriptGraphEdge* UScriptGraphNode::GetEdge(UScriptGraphNode* ChildNode)
{
	return Edges.Contains(ChildNode) ? Edges.FindChecked(ChildNode) : nullptr;
}

bool UScriptGraphNode::IsLeafNode() const
{
	return ChildrenNodes.Num() == 0;
}

UScriptGraph* UScriptGraphNode::GetGraph() const
{
	return Graph;
}

FText UScriptGraphNode::GetDescription_Implementation() const
{
	return LOCTEXT("NodeDesc", "Script Graph Node");
}

#if WITH_EDITOR
bool UScriptGraphNode::IsNameEditable() const
{
	return true;
}

FLinearColor UScriptGraphNode::GetBackgroundColor() const
{
	return BackgroundColor;
}

FText UScriptGraphNode::GetNodeTitle() const
{
	return NodeTitle.IsEmpty() ? GetDescription() : NodeTitle;
}

FText UScriptGraphNode::GetContextMenuName() const
{
	return ContextMenuName;
}

const TSubclassOf<UScriptGraphNode>& UScriptGraphNode::GetCompatibleGraphType() const
{
	return CompatibleGraphType;
}

void UScriptGraphNode::SetNodeTitle(const FText& NewTitle)
{
	NodeTitle = NewTitle;
}

bool UScriptGraphNode::CanCreateConnection(UScriptGraphNode* Other, FText& ErrorMessage)
{
	return true;
}

bool UScriptGraphNode::CanCreateConnectionTo(UScriptGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage)
{
	if( ENodeLimit::Limited == ChildrenLimitType
		&& NumberOfChildrenNodes >= ChildrenLimit)
	{
		ErrorMessage = FText::FromString("Children limit exceeded");
		return false;
	}

	return CanCreateConnection(Other, ErrorMessage);
}

bool UScriptGraphNode::CanCreateConnectionFrom(UScriptGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage)
{
	if(ENodeLimit::Limited == ParentLimitType
		&& NumberOfParentNodes >= ParentLimit)
	{
		ErrorMessage = FText::FromString("Parent limit exceeded");
		return false;
	}

	return true;
}
#endif

#undef LOCTEXT_NAMESPACE
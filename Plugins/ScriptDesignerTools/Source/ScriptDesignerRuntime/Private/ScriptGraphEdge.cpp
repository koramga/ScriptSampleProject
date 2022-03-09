// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptGraphEdge.h"

UScriptGraphEdge::UScriptGraphEdge()
{
}

UScriptGraphEdge::~UScriptGraphEdge()
{
}

UScriptGraphNode* UScriptGraphEdge::GetStartNode() const
{
	return StartNode;
}

UScriptGraphNode* UScriptGraphEdge::GetEndNode() const
{
	return EndNode;
}

void UScriptGraphEdge::SetStartNode(UScriptGraphNode* InStartNode)
{
	StartNode = InStartNode;
}

void UScriptGraphEdge::SetEndNode(UScriptGraphNode* InEndNode)
{
	EndNode = InEndNode;
}

UScriptGraph* UScriptGraphEdge::GetGraph() const
{
	return Graph;
}

void UScriptGraphEdge::SetGraph(UScriptGraph* InGraph)
{
	Graph = InGraph;
}

#if WITH_EDITOR
void UScriptGraphEdge::SetNodeTitle(const FText& NewTitle)
{
	NodeTitle = NewTitle;
}
#endif

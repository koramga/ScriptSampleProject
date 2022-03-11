// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoLayout/TreeLayoutStrategy.h"

UTreeLayoutStrategy::UTreeLayoutStrategy()
{

}

UTreeLayoutStrategy::~UTreeLayoutStrategy()
{
}

void UTreeLayoutStrategy::Layout(UEdGraph* InEdGraph)
{
}

void UTreeLayoutStrategy::InitPass(UScriptGraphNode* RootNode, const FVector2D& Anchor)
{
}

bool UTreeLayoutStrategy::ResolveConflictPass(UScriptGraphNode* Node)
{
	return false;
}

bool UTreeLayoutStrategy::ResolveConflict(UScriptGraphNode* LRoot, UScriptGraphNode* RRoot)
{
	return false;
}

void UTreeLayoutStrategy::GetLeftContour(UScriptGraphNode* RootNode, int32 Level, TArray<UEdNode_ScriptGraphNode*>& Contour)
{
}

void UTreeLayoutStrategy::GetRightContour(UScriptGraphNode* RootNode, int32 Level, TArray<UEdNode_ScriptGraphNode*>& Contour)
{
}

void UTreeLayoutStrategy::ShiftSubTree(UScriptGraphNode* RootNode, const FVector2D& Offset)
{
}

void UTreeLayoutStrategy::UpdateParentNodePosition(UScriptGraphNode* RootNode)
{
}

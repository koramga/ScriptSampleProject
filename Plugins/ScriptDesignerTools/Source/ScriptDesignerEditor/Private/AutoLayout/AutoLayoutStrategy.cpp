// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoLayout/AutoLayoutStrategy.h"
#include "Kismet/KismetMathLibrary.h"
#include "ScriptDesignerEditor/Node/EdGraphNode_BaseScriptNode.h"

UAutoLayoutStrategy::UAutoLayoutStrategy()
{
    Settings = nullptr;
    MaxIteration = 50;
    OptimalDistance = 150;
}

UAutoLayoutStrategy::~UAutoLayoutStrategy()
{

}

int32 UAutoLayoutStrategy::GetNodeWidth(UEdNode_ScriptGraphNode* EdNode)
{
    return 0;
}

int32 UAutoLayoutStrategy::GetNodeHeight(UEdNode_ScriptGraphNode* EdNode)
{
    return 0;
}

FBox2D UAutoLayoutStrategy::GetNodeBound(UEdGraphNode* EdNode)
{
    return FBox2D();
}

FBox2D UAutoLayoutStrategy::GetActualBounds(UScriptGraphNode* RootNode)
{
    return FBox2D();
}

void UAutoLayoutStrategy::RandomLayoutOnTree(UScriptGraphNode* RootNode, const FBox2D& Bound)
{
}

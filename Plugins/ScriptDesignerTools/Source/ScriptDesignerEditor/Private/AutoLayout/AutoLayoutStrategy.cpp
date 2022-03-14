// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoLayout/AutoLayoutStrategy.h"
#include "Kismet/KismetMathLibrary.h"
#include "ScriptDesignerEditor/Node/EdNode_ScriptGraphNode.h"
#include "ScriptDesignerEditor/Node/SEdNode_ScriptGraphNode.h"

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
    return EdNode->GetEdNode()->GetCachedGeometry().GetLocalSize().X;
}

int32 UAutoLayoutStrategy::GetNodeHeight(UEdNode_ScriptGraphNode* EdNode)
{
    return EdNode->GetEdNode()->GetCachedGeometry().GetLocalSize().Y;
}

FBox2D UAutoLayoutStrategy::GetNodeBound(UEdGraphNode* EdNode)
{
    int32 NodeWidth = GetNodeWidth(Cast<UEdNode_ScriptGraphNode>(EdNode));
    int32 NodeHeight = GetNodeHeight(Cast<UEdNode_ScriptGraphNode>(EdNode));
    FVector2D Min(EdNode->NodePosX, EdNode->NodePosY);
    FVector2D Max(EdNode->NodePosX + NodeWidth, EdNode->NodePosY + NodeHeight);
    return FBox2D(Min, Max);
}

FBox2D UAutoLayoutStrategy::GetActualBounds(UScriptGraphNode* RootNode)
{
    int32 Level = 0;
    TArray<UScriptGraphNode*> CurrLevelNodes = { RootNode };
    TArray<UScriptGraphNode*> NextLevelNodes;

    FBox2D Rtn = GetNodeBound(EdGraph->FindNode(RootNode));

    while (CurrLevelNodes.Num() != 0)
    {
        for (int i = 0; i < CurrLevelNodes.Num(); ++i)
        {
            UScriptGraphNode* Node = CurrLevelNodes[i];
            check(Node != nullptr);

            Rtn += GetNodeBound(EdGraph->FindNode(Node));

            const TArray<UScriptGraphNode*>& ChildrenNodes = Node->GetChildrenNodes();

            for (UScriptGraphNode* ChildNode : ChildrenNodes)
            {
                NextLevelNodes.Add(ChildNode);
            }
        }

        CurrLevelNodes = NextLevelNodes;
        NextLevelNodes.Reset();
        ++Level;
    }

    return Rtn;
}

void UAutoLayoutStrategy::RandomLayoutOnTree(UScriptGraphNode* RootNode, const FBox2D& Bound)
{
    int32 Level = 0;
    TArray<UScriptGraphNode*> CurrLevelNodes = { RootNode };
    TArray<UScriptGraphNode*> NextLevelNodes;

    while (CurrLevelNodes.Num() != 0)
    {
        for (int i = 0; i < CurrLevelNodes.Num(); ++i)
        {
            UScriptGraphNode* Node = CurrLevelNodes[i];
            check(Node != nullptr);

            UEdNode_ScriptGraphNode* EdNode_Node = EdGraph->FindNode(Node);

            EdNode_Node->NodePosX = UKismetMathLibrary::RandomFloatInRange(Bound.Min.X, Bound.Max.X);
            EdNode_Node->NodePosY = UKismetMathLibrary::RandomFloatInRange(Bound.Min.Y, Bound.Max.Y);

            const TArray<UScriptGraphNode*>& ChildrenNodes = Node->GetChildrenNodes();

            for (UScriptGraphNode* ChildNode : ChildrenNodes)
            {
                NextLevelNodes.Add(ChildNode);
            }
        }

        CurrLevelNodes = NextLevelNodes;
        NextLevelNodes.Reset();
        ++Level;
    }
}

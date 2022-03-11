// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/AssetGraphSchema_ScriptGraph.h"
#include "ToolMenus.h"
#include "ScriptGraphEditorPCH.h"
#include "ScriptDesignerEditor/EdNode_ScriptGraphNode.h"
#include "ScriptDesignerEditor/EdNode_ScriptGraphEdge.h"
#include "ScriptDesignerEditor/ConnectionDrawingPolicy_ScriptGraph.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"
//#include "AutoLayout/ForceDirectedLayoutStrategy.h"
//#include "AutoLayout/TreeLayoutStrategy.h"

UEdGraphNode* FAssetSchemaAction_ScriptGraph_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2D Location, bool bSelectNewNode)
{
	return FEdGraphSchemaAction::PerformAction(ParentGraph, FromPin, Location, bSelectNewNode);
}

void FAssetSchemaAction_ScriptGraph_NewNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
}

UEdGraphNode* FAssetSchemaAction_ScriptGraph_NewEdge::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2D Location, bool bSelectNewNode)
{
	return FEdGraphSchemaAction::PerformAction(ParentGraph, FromPin, Location, bSelectNewNode);
}

void FAssetSchemaAction_ScriptGraph_NewEdge::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
}

void UAssetGraphSchema_ScriptGraph::GetBreakLinkToSubMenuActions(UToolMenu* Menu, UEdGraphPin* InGraphPin)
{
}

EGraphType UAssetGraphSchema_ScriptGraph::GetGraphType(const UEdGraph* TestEdGraph) const
{
	return Super::GetGraphType(TestEdGraph);
}

void UAssetGraphSchema_ScriptGraph::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	Super::GetGraphContextActions(ContextMenuBuilder);
}

void UAssetGraphSchema_ScriptGraph::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetContextMenuActions(Menu, Context);
}

const FPinConnectionResponse UAssetGraphSchema_ScriptGraph::CanCreateConnection(const UEdGraphPin* A,
	const UEdGraphPin* B) const
{
	return Super::CanCreateConnection(A, B);
}

bool UAssetGraphSchema_ScriptGraph::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	return Super::TryCreateConnection(A, B);
}

bool UAssetGraphSchema_ScriptGraph::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const
{
	return Super::CreateAutomaticConversionNodeAndConnections(A, B);
}

FConnectionDrawingPolicy* UAssetGraphSchema_ScriptGraph::CreateConnectionDrawingPolicy(int32 InBackLayerID,
	int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements,
	UEdGraph* InGraphObj) const
{
	return Super::CreateConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect,
	                                            InDrawElements, InGraphObj);
}

FLinearColor UAssetGraphSchema_ScriptGraph::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return Super::GetPinTypeColor(PinType);
}

void UAssetGraphSchema_ScriptGraph::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	Super::BreakNodeLinks(TargetNode);
}

void UAssetGraphSchema_ScriptGraph::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const
{
	Super::BreakPinLinks(TargetPin, bSendsNodeNotifcation);
}

void UAssetGraphSchema_ScriptGraph::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
	Super::BreakSinglePinLink(SourcePin, TargetPin);
}

UEdGraphPin* UAssetGraphSchema_ScriptGraph::DropPinOnNode(UEdGraphNode* InTargetNode, const FName& InSourcePinName,
	const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection) const
{
	return Super::DropPinOnNode(InTargetNode, InSourcePinName, InSourcePinType, InSourcePinDirection);
}

bool UAssetGraphSchema_ScriptGraph::SupportsDropPinOnNode(UEdGraphNode* InTargetNode,
	const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection, FText& OutErrorMessage) const
{
	return Super::SupportsDropPinOnNode(InTargetNode, InSourcePinType, InSourcePinDirection, OutErrorMessage);
}

bool UAssetGraphSchema_ScriptGraph::IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const
{
	return Super::IsCacheVisualizationOutOfDate(InVisualizationCacheID);
}

int32 UAssetGraphSchema_ScriptGraph::GetCurrentVisualizationCacheID() const
{
	return Super::GetCurrentVisualizationCacheID();
}

void UAssetGraphSchema_ScriptGraph::ForceVisualizationCacheClear() const
{
	Super::ForceVisualizationCacheClear();
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "AssetGraphSchema_ScriptGraph.generated.h"

/**
 * 
 */

class UEdNode_ScriptGraphNode;
class UEdNode_ScriptGraphEdge;
class UAutoLayoutStrategy;

USTRUCT()
struct SCRIPTDESIGNEREDITOR_API FAssetSchemaAction_ScriptGraph_NewNode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

public :
	FAssetSchemaAction_ScriptGraph_NewNode()
		: NodeTemplate(nullptr) {}
	
	FAssetSchemaAction_ScriptGraph_NewNode(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
	: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr) {}

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	UEdNode_ScriptGraphNode* NodeTemplate;
};

USTRUCT()
struct SCRIPTDESIGNEREDITOR_API FAssetSchemaAction_ScriptGraph_NewEdge : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();
	
public :
	FAssetSchemaAction_ScriptGraph_NewEdge(): NodeTemplate(nullptr){}
	FAssetSchemaAction_ScriptGraph_NewEdge(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr) {}

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	
	UEdNode_ScriptGraphEdge* NodeTemplate;
};

UCLASS()
class SCRIPTDESIGNEREDITOR_API UAssetGraphSchema_ScriptGraph : public UEdGraphSchema
{
	GENERATED_BODY()

public :
	void GetBreakLinkToSubMenuActions(class UToolMenu* Menu, class UEdGraphPin* InGraphPin);
	virtual EGraphType GetGraphType(const UEdGraph* TestEdGraph) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual class FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const override;
	virtual void BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const override;
	virtual UEdGraphPin* DropPinOnNode(UEdGraphNode* InTargetNode, const FName& InSourcePinName, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection) const override;
	virtual bool SupportsDropPinOnNode(UEdGraphNode* InTargetNode, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection, FText& OutErrorMessage) const override;
	virtual bool IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const override;
	virtual int32 GetCurrentVisualizationCacheID() const override;
	virtual void ForceVisualizationCacheClear() const override;

private :
	static int32 CurrentCacheRefreshID;
};

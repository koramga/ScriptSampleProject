#include "ScriptDesignerEditor/ConnectionDrawingPolicy_BlueprintScriptGraph.h"

#include "Edge/EdNode_ScriptGraphEdge.h"

int32 FConnectionDrawingPolicy_BlueprintScriptGraph::FindGraphNodeIndexFromArrangedWidget(
	FArrangedChildren& ArrangedNodes, UEdGraphNode* InGraphNode)
{
	for(int32 NodeIndex = 0; NodeIndex < ArrangedNodes.Num(); ++NodeIndex)
	{
		FArrangedWidget& CurWidget = ArrangedNodes[NodeIndex];
		TSharedRef<SGraphNode> ChildNode = StaticCastSharedRef<SGraphNode>(CurWidget.Widget);

		if(ChildNode->GetNodeObj() == InGraphNode)
		{
			return NodeIndex;
		}
	}

	return -1;
}

FConnectionDrawingPolicy_BlueprintScriptGraph::FConnectionDrawingPolicy_BlueprintScriptGraph(int32 InBackLayerID,
                                                                                             int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements,
                                                                                             UEdGraph* InGraphObj)
	: FConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements), GraphObj(InGraphObj)
{
	
}

void FConnectionDrawingPolicy_BlueprintScriptGraph::DetermineLinkGeometry(FArrangedChildren& ArrangedNodes,
	TSharedRef<SWidget>& OutputPinWidget, UEdGraphPin* OutputPin, UEdGraphPin* InputPin,
	FArrangedWidget*& StartWidgetGeometry, FArrangedWidget*& EndWidgetGeometry)
{
	if(UEdNode_ScriptGraphEdge* EdgeNode = Cast<UEdNode_ScriptGraphEdge>(InputPin->GetOwningNode()))
	{
		UEdNode_ScriptGraphNode* Start = EdgeNode->GetStartNode();
		UEdNode_ScriptGraphNode* End = EdgeNode->GetEndNode();

		if(Start != nullptr && End != nullptr)
		{			
			int32 StartNodeIndex = FindGraphNodeIndexFromArrangedWidget(ArrangedNodes, Start);
			int32 EndNodeIndex = FindGraphNodeIndexFromArrangedWidget(ArrangedNodes, End);
			
			if(StartNodeIndex >= 0 && EndNodeIndex >= 0)
			{
				StartWidgetGeometry = &(ArrangedNodes[StartNodeIndex]);
				EndWidgetGeometry = &(ArrangedNodes[EndNodeIndex]);
			}
		}
	}
	else
	{
		StartWidgetGeometry = PinGeometries->Find(OutputPinWidget);

		if(TSharedPtr<SGraphPin>* pTargetWidget = PinToPinWidgetMap.Find(InputPin))
		{
			TSharedRef<SGraphPin> InputWidget = (*pTargetWidget).ToSharedRef();
			EndWidgetGeometry = PinGeometries->Find(InputWidget);
		}
	}
}

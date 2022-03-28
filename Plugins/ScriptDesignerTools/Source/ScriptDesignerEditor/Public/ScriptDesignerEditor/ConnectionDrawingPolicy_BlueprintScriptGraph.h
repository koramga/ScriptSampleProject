#pragma once

#include "ConnectionDrawingPolicy.h"

class FConnectionDrawingPolicy_BlueprintScriptGraph : public FConnectionDrawingPolicy
{
protected:
	UEdGraph* GraphObj;

private :
	int32 FindGraphNodeIndexFromArrangedWidget(FArrangedChildren& ArrangedNodes, UEdGraphNode* InGraphNode);
	
public :
	FConnectionDrawingPolicy_BlueprintScriptGraph(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj);
	
	virtual void DetermineLinkGeometry(FArrangedChildren& ArrangedNodes, TSharedRef<SWidget>& OutputPinWidget, UEdGraphPin* OutputPin, UEdGraphPin* InputPin, FArrangedWidget*& StartWidgetGeometry, FArrangedWidget*& EndWidgetGeometry) override;
};

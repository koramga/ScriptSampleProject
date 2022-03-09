// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/ScriptGraphDragConnection.h"
#include "Widgets/SBoxPanel.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Images/SImage.h"
#include "EdGraph/EdGraph.h"
#include "SGraphPanel.h"
#include "ScopedTransaction.h"
#include "ScriptDesignerEditor/EdNode_ScriptGraphNode.h"
#include "ScriptDesignerEditor/EdNode_ScriptGraphEdge.h"

TSharedRef<FScriptGraphDragConnection> FScriptGraphDragConnection::New(const TSharedRef<SGraphPanel>& GraphPanel, const FDraggedPinTable& DraggedPins)
{
	TSharedRef<FScriptGraphDragConnection> Operation = MakeShareable(new FScriptGraphDragConnection(GraphPanel, DraggedPins));
	Operation->Construct();

	return Operation;
}

void FScriptGraphDragConnection::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
	GraphPanel->OnStopMakingConnection();
	
	FGraphEditorDragDropAction::OnDrop(bDropWasHandled, MouseEvent);
}

void FScriptGraphDragConnection::HoverTargetChanged()
{
	TArray<FPinConnectionResponse> UniqueMessage;

	if(UEdGraphPin* TargetPinObj = GetHoveredPin())
	{
		TArray<UEdGraphPin*> ValidSourcePins;
		ValidateGraphPinList(ValidSourcePins);

		//Check the schma for connection responses
		for(UEdGraphPin* StartingPinObj : ValidSourcePins)
		{
			//The Graph object in which the pins resize.
			UEdGraph* GraphObj = StartingPinObj->GetOwningNode()->GetGraph();

			//Determine what the schma thinks about the writing action.
			const FPinConnectionResponse Response = GraphObj->GetSchema()->CanCreateConnection(StartingPinObj, TargetPinObj);

			if(Response.Response == ECanCreateConnectionResponse::CONNECT_RESPONSE_DISALLOW)
			{
				TSharedPtr<SGraphNode> NodeWidget = TargetPinObj->GetOwningNode()->DEPRECATED_NodeWidget.Pin();

				if(NodeWidget.IsValid())
				{
					NodeWidget->NotifyDisallowedPinConnection(StartingPinObj, TargetPinObj);
				}
			}

			UniqueMessage.AddUnique(Response);
		}
	}
	else if(UEdNode_ScriptGraphNode* TargetNodeObj = Cast<UEdNode_ScriptGraphNode>(GetHoveredNode()))
	{
		TArray<UEdGraphPin*> ValidSourcePins;
		ValidateGraphPinList(ValidSourcePins);

		for(UEdGraphPin* StartingPinObj : ValidSourcePins)
		{
			FPinConnectionResponse Response;
			FText ResponseText;

			const UEdGraphSchema* Schema = StartingPinObj->GetSchema();
			//UEdGraphPin* TargetPin = TargetNodeObj->GetInputPin();
		}
	}
	
	FGraphEditorDragDropAction::HoverTargetChanged();
}

FReply FScriptGraphDragConnection::DroppedOnPin(FVector2D ScreenPosition, FVector2D GraphPosition)
{
	return FGraphEditorDragDropAction::DroppedOnPin(ScreenPosition, GraphPosition);
}

FReply FScriptGraphDragConnection::DroppedOnNode(FVector2D ScreenPosition, FVector2D GraphPosition)
{
	return FGraphEditorDragDropAction::DroppedOnNode(ScreenPosition, GraphPosition);
}

FReply FScriptGraphDragConnection::DroppedOnPanel(const TSharedRef<SWidget>& Panel, FVector2D ScreenPosition,
	FVector2D GraphPosition, UEdGraph& Graph)
{
	return FGraphEditorDragDropAction::DroppedOnPanel(Panel, ScreenPosition, GraphPosition, Graph);
}

void FScriptGraphDragConnection::OnDragged(const FDragDropEvent& DragDropEvent)
{
	FVector2D TargetPosition = DragDropEvent.GetScreenSpacePosition();

	//Reposition the info window wrt to the drag
	CursorDecoratorWindow->MoveWindowTo(DragDropEvent.GetScreenSpacePosition() + DecoratorAdjust);
	//Request the active panel to scroll if required.
	GraphPanel->RequestDeferredPan(TargetPosition);
	
	FGraphEditorDragDropAction::OnDragged(DragDropEvent);
}

void FScriptGraphDragConnection::ValidateGraphPinList(TArray<UEdGraphPin*>& OutValidPins)
{
}

FScriptGraphDragConnection::FScriptGraphDragConnection(const TSharedRef<SGraphPanel>& GraphPanel,
                                                       const FDraggedPinTable& DraggedPins)
{
}

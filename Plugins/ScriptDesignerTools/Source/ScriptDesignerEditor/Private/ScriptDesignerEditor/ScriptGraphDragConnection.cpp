// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/ScriptGraphDragConnection.h"
#include "Widgets/SBoxPanel.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Images/SImage.h"
#include "EdGraph/EdGraph.h"
#include "SGraphPanel.h"
#include "ScopedTransaction.h"

TSharedRef<FScriptGraphDragConnection> FScriptGraphDragConnection::New(const TSharedRef<SGraphPanel>& InGraphPanel,
	const FDraggedPinTable& InStartingPins)
{
	TSharedRef<FScriptGraphDragConnection> Operation = MakeShareable(new FScriptGraphDragConnection(InGraphPanel, InStartingPins));
	Operation->Construct();
	
	return Operation;
}

void FScriptGraphDragConnection::OnDrop(bool bDropWasHandled, const FPointerEvent& MouseEvent)
{
	FGraphEditorDragDropAction::OnDrop(bDropWasHandled, MouseEvent);
}

void FScriptGraphDragConnection::HoverTargetChanged()
{
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
	FGraphEditorDragDropAction::OnDragged(DragDropEvent);
}

void FScriptGraphDragConnection::ValidateGraphPinList(TArray<UEdGraphPin*>& OutValidPins)
{
}

FScriptGraphDragConnection::FScriptGraphDragConnection(const TSharedRef<SGraphPanel>& GraphPanel,
                                                       const FDraggedPinTable& DraggedPins)
{
}

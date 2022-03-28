// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/Pin/SGraphPin_ScriptPin.h"

#include "ScriptDesignerEditor/Colors_ScriptGraph.h"
#include "ScriptDesignerEditor/ScriptGraphDragConnection.h"

void SGraphPin_ScriptPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	this->SetCursor(EMouseCursor::Default);

	bShowLabel = true;

	GraphPinObj = InPin;
	check(GraphPinObj != nullptr);

	const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
	check(Schema);

	SBorder::Construct(SBorder::FArguments()
		.BorderImage(this, &SGraphPin_ScriptPin::GetPinBorder)
		.BorderBackgroundColor(this, &SGraphPin_ScriptPin::GetPinColor)
		.OnMouseButtonDown(this, &SGraphPin_ScriptPin::OnPinMouseDown)
		.Cursor(this, &SGraphPin_ScriptPin::GetPinCursor)
		.Padding(FMargin(5.f))
	);
}

FSlateColor SGraphPin_ScriptPin::GetPinColor() const
{
	return ScriptGraphColor::Pin::Default;
}

TSharedRef<SWidget> SGraphPin_ScriptPin::GetDefaultValueWidget()
{
	return SNew(STextBlock);
}

const FSlateBrush* SGraphPin_ScriptPin::GetPinBorder() const
{
		return FEditorStyle::GetBrush(TEXT("Graph.StateNode.Body"));
}

TSharedRef<FDragDropOperation> SGraphPin_ScriptPin::SpawnPinDragEvent(const TSharedRef<SGraphPanel>& InGraphPanel,
	const TArray<TSharedRef<SGraphPin>>& InStartingPins)
{
	FScriptGraphDragConnection::FDraggedPinTable PinHandles;
	PinHandles.Reserve(InStartingPins.Num());

	//since the graph can be refreshed and pins can be reconstructed/replaced
	//behind the scenes, the DragDropOperation holds onto FGraphPinHandles
	//instead of direct widgets/graph-pins.
	for(const TSharedRef<SGraphPin>& PinWidget : InStartingPins)
	{
		PinHandles.Add(PinWidget->GetPinObj());
	}

	return FScriptGraphDragConnection::New(InGraphPanel, PinHandles);
}
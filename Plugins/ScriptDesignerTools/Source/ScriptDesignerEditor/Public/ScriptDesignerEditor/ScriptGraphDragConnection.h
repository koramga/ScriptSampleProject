// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Input/DragAndDrop.h"
#include "Input/Reply.h"
#include "Widgets/SWidget.h"
#include "SGraphPin.h"
#include "GraphEditorDragDropAction.h"

/**
 * 
 */

class FScriptGraphDragConnection : public FGraphEditorDragDropAction
{
public :
    DRAG_DROP_OPERATOR_TYPE(FDragConnection, FGraphEditorDragDropAction)

	typedef TArray<FGraphPinHandle> FDraggedPinTable;
	static TSharedRef<FScriptGraphDragConnection> New(const TSharedRef<SGraphPanel>& InGraphPanel, const FDraggedPinTable& InStartingPins);
	
	
};
// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/Node/SGraphNode_ScriptNode.h"
#include "GraphEditorSettings.h"
#include "SCommentBubble.h"
#include "ScriptGraph.h"
#include "SGraphPin.h"
#include "ScriptDesignerEditor/Colors_ScriptGraph.h"
#include "ScriptDesignerEditor/ScriptGraphDragConnection.h"
#include "ScriptDesignerEditor/Node/EdNode_ScriptGraphNode.h"
#include "ScriptDesignerEditor/Colors_ScriptGraph.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

#define LOCTEXT_NAMESPACE "EdNode_ScriptGraph"

void SGraphNode_ScriptNode::Construct(const FArguments& InArgs, UEdGraphNode_ScriptNode* InNode)
{
	
}

void SGraphNode_ScriptNode::UpdateGraphNode()
{
	SGraphNode_BaseScriptNode::UpdateGraphNode();
}

void SGraphNode_ScriptNode::CreatePinWidgets()
{
	SGraphNode_BaseScriptNode::CreatePinWidgets();
}

void SGraphNode_ScriptNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	SGraphNode_BaseScriptNode::AddPin(PinToAdd);
}

bool SGraphNode_ScriptNode::IsNameReadOnly() const
{
	return SGraphNode_BaseScriptNode::IsNameReadOnly();
}

void SGraphNode_ScriptNode::OnNameTextCommited(const FString& InText, ETextCommit::Type CommitInfo)
{
}

FSlateColor SGraphNode_ScriptNode::GetBorderBackgroundColor() const
{
}

FSlateColor SGraphNode_ScriptNode::GetBackgroundColor() const
{
}

EVisibility SGraphNode_ScriptNode::GetDragOverMarkerVisibility() const
{
}

const FSlateBrush* SGraphNode_ScriptNode::GetNameIcon() const
{
}

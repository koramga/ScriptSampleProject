// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/SEdNode_ScriptGraphNode.h"


void SEdNode_ScriptGraphNode::Construct(const FArguments& InArgs, UEdNode_GenericGraphNode* InNode)
{
}

void SEdNode_ScriptGraphNode::UpdateGraphNode()
{
	SGraphNode::UpdateGraphNode();
}

void SEdNode_ScriptGraphNode::CreatePinWidgets()
{
	SGraphNode::CreatePinWidgets();
}

void SEdNode_ScriptGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	SGraphNode::AddPin(PinToAdd);
}

bool SEdNode_ScriptGraphNode::IsNameReadOnly() const
{
	return SGraphNode::IsNameReadOnly();
}

void SEdNode_ScriptGraphNode::OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo)
{
}

FSlateColor SEdNode_ScriptGraphNode::GetBorderBackgroundColor() const
{
}

FSlateColor SEdNode_ScriptGraphNode::GetBackgroundColor() const
{
}

EVisibility SEdNode_ScriptGraphNode::GetDragOverMarkerVisibility() const
{
}

const FSlateBrush* SEdNode_ScriptGraphNode::GetNameIcon() const
{
}

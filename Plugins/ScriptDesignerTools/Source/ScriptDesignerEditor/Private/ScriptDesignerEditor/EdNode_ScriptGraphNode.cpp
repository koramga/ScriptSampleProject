// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/EdNode_ScriptGraphNode.h"

#define LOCTEXT_NAMESPACE "EdNode_ScriptGraph"

UEdNode_ScriptGraphNode::UEdNode_ScriptGraphNode()
{
	bCanRenameNode = true;
}

UEdNode_ScriptGraphNode::~UEdNode_ScriptGraphNode()
{
}

void UEdNode_ScriptGraphNode::SetScriptGraphNode(UScriptGraphNode* InNode)
{
	ScriptGraphNode = InNode;
}

UScriptGraphNode* UEdNode_ScriptGraphNode::GetScriptGraphNode() const
{
	return ScriptGraphNode;
}

void UEdNode_ScriptGraphNode::SetEdNode(SEdNode_ScriptGraphNode* InNode)
{
	SEdNode = InNode;
}

SEdNode_ScriptGraphNode* UEdNode_ScriptGraphNode::GetEdNode() const
{
	return SEdNode;
}

void UEdNode_ScriptGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out"));
}

UEdGraph_ScriptGraph* UEdNode_ScriptGraphNode::GetScriptGraphEdGraph() const
{
	return Cast<UEdGraph_ScriptGraph>(GetGraph());
}

FText UEdNode_ScriptGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if(nullptr == ScriptGraphNode)
	{
		return Super::GetNodeTitle(TitleType);
	}
	else
	{
		return ScriptGraphNode->GetNodeTitle();
	}
}

void UEdNode_ScriptGraphNode::PrepareForCopying()
{
	ScriptGraphNode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdNode_ScriptGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if(nullptr != FromPin)
	{
		if(GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}

FLinearColor UEdNode_ScriptGraphNode::GetBackgroundColor() const
{
	return ScriptGraphNode == nullptr ? FLinearColor::Black : ScriptGraphNode->GetBackgroundColor();
}

UEdGraphPin* UEdNode_ScriptGraphNode::GetInputPin() const
{
	return Pins[0];
}

UEdGraphPin* UEdNode_ScriptGraphNode::GetOutputPin() const
{
	return Pins[1];
}

void UEdNode_ScriptGraphNode::PostEditUndo()
{
	Super::PostEditUndo();
}

#undef LOCTEXT_NAMESPACE
// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/Node/EdGraphNode_BaseScriptNode.h"
#include "ScriptDesignerEditor/EdGraph_ScriptGraph.h"

#define LOCTEXT_NAMESPACE "EdNode_ScriptGraph"

UEdGraphNode_BaseScriptNode::UEdGraphNode_BaseScriptNode()
{
	bCanRenameNode = true;
}

UEdGraphNode_BaseScriptNode::~UEdGraphNode_BaseScriptNode()
{
}

void UEdGraphNode_BaseScriptNode::SetScriptGraphNode(UScriptGraphNode* InNode)
{
	ScriptGraphNode = InNode;
}

UScriptGraphNode* UEdGraphNode_BaseScriptNode::GetScriptGraphNode() const
{
	return ScriptGraphNode;
}

void UEdGraphNode_BaseScriptNode::SetEdNode(SGraphNode_BaseScriptNode* InNode)
{
	SEdNode = InNode;
}

SGraphNode_BaseScriptNode* UEdGraphNode_BaseScriptNode::GetEdNode() const
{
	return SEdNode;
}

void UEdGraphNode_BaseScriptNode::NewScriptGraphNode(UObject* Outer, const UClass* Class)
{
	if(false == IsValid(ScriptGraphNode))
		ScriptGraphNode = NewObject<UScriptGraphNode>(Outer, Class);
}

UEdGraph_ScriptGraph* UEdGraphNode_BaseScriptNode::GetScriptGraphEdGraph() const
{
	return Cast<UEdGraph_ScriptGraph>(GetGraph());
}

void UEdGraphNode_BaseScriptNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out"));
}

FText UEdGraphNode_BaseScriptNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
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

void UEdGraphNode_BaseScriptNode::PrepareForCopying()
{
	ScriptGraphNode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdGraphNode_BaseScriptNode::AutowireNewNode(UEdGraphPin* FromPin)
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

FLinearColor UEdGraphNode_BaseScriptNode::GetBackgroundColor() const
{
	return ScriptGraphNode == nullptr ? FLinearColor::Black : ScriptGraphNode->GetBackgroundColor();
}

UEdGraphPin* UEdGraphNode_BaseScriptNode::GetInputPin() const
{
	return Pins[0];
}

UEdGraphPin* UEdGraphNode_BaseScriptNode::GetOutputPin() const
{
	return Pins[1];
}

void UEdGraphNode_BaseScriptNode::PostEditUndo()
{
	Super::PostEditUndo();
}

#undef LOCTEXT_NAMESPACE

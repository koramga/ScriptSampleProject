// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/Edge/EdNode_ScriptGraphEdge.h"

#include "ScriptGraphEdge.h"
#include "ScriptDesignerEditor/Node/EdGraphNode_BaseScriptNode.h"

UEdNode_ScriptGraphEdge::UEdNode_ScriptGraphEdge()
{
	bCanRenameNode = true;
}

void UEdNode_ScriptGraphEdge::SetEdge(UScriptGraphEdge* Edge)
{
	ScriptGraphEdge = Edge;
}

UScriptGraphEdge* UEdNode_ScriptGraphEdge::GetEdge() const
{
	return ScriptGraphEdge;
}

void UEdNode_ScriptGraphEdge::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Edge"), FName(), TEXT("In"));
	Inputs->bHidden = true;
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Edge"), FName(), TEXT("Out"));
	Outputs->bHidden = true;
}

FText UEdNode_ScriptGraphEdge::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if(ScriptGraphEdge)
	{
		return ScriptGraphEdge->GetNodeTitle();
	}

	return FText();
}

void UEdNode_ScriptGraphEdge::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if(Pin->LinkedTo.Num() == 0)
	{
		//Commit suicide; transitions must always have an input and output connection.
		Modify();

		//Our parent graph will have our graph in SubGraphs so needs to be modified to record that.
		if(UEdGraph* ParentGraph = GetGraph())
		{
			ParentGraph->Modify();
		}

		DestroyNode();
	}
}

void UEdNode_ScriptGraphEdge::PrepareForCopying()
{
	ScriptGraphEdge->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdNode_ScriptGraphEdge::CreateConnection(UEdGraphNode_BaseScriptNode* Start, UEdGraphNode_BaseScriptNode* End)
{
	Pins[0]->Modify();
	Pins[0]->LinkedTo.Empty();

	Start->GetOutputPin()->Modify();
	Pins[0]->MakeLinkTo(Start->GetOutputPin());

	//This to next
	Pins[1]->Modify();
	Pins[1]->LinkedTo.Empty();

	End->GetInputPin()->Modify();
	Pins[1]->MakeLinkTo((End->GetInputPin()));
}

UEdGraphNode_BaseScriptNode* UEdNode_ScriptGraphEdge::GetStartNode()
{
	if(Pins[0]->LinkedTo.Num() > 0)
	{
		return Cast<UEdGraphNode_BaseScriptNode>(Pins[0]->LinkedTo[0]->GetOwningNode());
	}
	else
	{
		return nullptr;
	}
}

UEdGraphNode_BaseScriptNode* UEdNode_ScriptGraphEdge::GetEndNode()
{
	if(Pins[1]->LinkedTo.Num() > 0)
	{
		return Cast<UEdGraphNode_BaseScriptNode>(Pins[1]->LinkedTo[0]->GetOwningNode());
	}
	else
	{
		return nullptr;
	}
}

#undef LOCTEXT_NAMESPACE
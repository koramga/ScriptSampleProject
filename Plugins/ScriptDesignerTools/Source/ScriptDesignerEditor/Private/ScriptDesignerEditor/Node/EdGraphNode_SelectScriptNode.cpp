// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/Node/EdGraphNode_SelectScriptNode.h"

void UEdGraphNode_SelectScriptNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out2"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out3"));
}
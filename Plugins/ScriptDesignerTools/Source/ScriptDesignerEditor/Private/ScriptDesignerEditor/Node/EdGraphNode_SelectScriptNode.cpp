// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/Node/EdGraphNode_SelectScriptNode.h"

void UEdGraphNode_SelectScriptNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("Start"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("True"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("False"));
}
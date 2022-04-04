// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/Node/EdGraphNode_SelectScriptNode.h"

void UEdGraphNode_SelectScriptNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "WorkNode", FName(), TEXT("Work"));
	CreatePin(EGPD_Output, "TrueNode", FName(), TEXT("True"));
	CreatePin(EGPD_Output, "FalseNode", FName(), TEXT("False"));
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdNode_ScriptGraphNode.h"
#include "EdGraph/EdGraphNode.h"
#include "EdNode_ScriptGraphEdge.generated.h"

class UScriptGraphNode;
class UScriptGraphEdge;
class UEdNode_ScriptGraphNode;

/**
 * 
 */
UCLASS()
class SCRIPTDESIGNEREDITOR_API UEdNode_ScriptGraphEdge : public UEdGraphNode
{
	GENERATED_BODY()

public :
	UEdNode_ScriptGraphEdge();

protected:
	UPROPERTY()
	class UEdGraph* Graph;

	UPROPERTY(VisibleAnywhere, Instanced, Category="ScriptGraph")
	UScriptGraphEdge* ScriptGraphEdge;

public :
	void SetEdge(UScriptGraphEdge* Edge);
	UScriptGraphEdge* GetEdge() const;
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual void PrepareForCopying() override;
	virtual UEdGraphPin* GetInputPin() const { return Pins[0]; }
	virtual UEdGraphPin* GetOutputPin() const { return Pins[1]; }

	void CreateConnection(UEdNode_ScriptGraphNode* Start, UEdNode_ScriptGraphNode* End);

	UEdNode_ScriptGraphNode* GetStartNode();
	UEdNode_ScriptGraphNode* GetEndNode();
};

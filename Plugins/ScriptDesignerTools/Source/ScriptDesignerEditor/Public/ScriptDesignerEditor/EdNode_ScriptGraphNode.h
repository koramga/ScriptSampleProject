// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph_ScriptGraph.h"
#include "ScriptGraphNode.h"
#include "EdGraph/EdGraphNode.h"
#include "EdNode_ScriptGraphNode.generated.h"

class UEdNode_ScriptGraphEdge;
class UEdNode_ScriptGraphNode;
class SEdNode_ScriptGraphNode;

/**
 * 
 */
UCLASS()
class SCRIPTDESIGNEREDITOR_API UEdNode_ScriptGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public :
	UEdNode_ScriptGraphNode();
	virtual ~UEdNode_ScriptGraphNode();

protected:
	UPROPERTY(VisibleAnywhere, Instanced, Category="ScriptGraph")
	UScriptGraphNode* ScriptGraphNode;
	
	SEdNode_ScriptGraphNode* SEdNode;

public:
	void SetScriptGraphNode(UScriptGraphNode* InNode);
	UScriptGraphNode* GetScriptGraphNode() const;

	void SetEdNode(SEdNode_ScriptGraphNode* InNode);
	SEdNode_ScriptGraphNode* GetEdNode() const;

	void NewScriptGraphNode(UObject* Outer, const UClass* Class);
	
	UEdGraph_ScriptGraph* GetScriptGraphEdGraph() const;

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PrepareForCopying() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

	virtual FLinearColor GetBackgroundColor() const;
	virtual UEdGraphPin* GetInputPin() const;
	virtual UEdGraphPin* GetOutputPin() const;

#if WITH_EDITOR
	virtual void PostEditUndo() override;
#endif	
};

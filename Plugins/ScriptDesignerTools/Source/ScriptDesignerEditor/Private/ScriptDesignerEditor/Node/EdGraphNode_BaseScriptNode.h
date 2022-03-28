// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScriptGraphNode.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraphNode_BaseScriptNode.generated.h"

class SGraphNode_BaseScriptNode;
class UEdGraph_ScriptGraph;

/**
 * 
 */
UCLASS()
class SCRIPTDESIGNEREDITOR_API UEdGraphNode_BaseScriptNode : public UEdGraphNode
{
	GENERATED_BODY()

public :
	UEdGraphNode_BaseScriptNode();
	virtual ~UEdGraphNode_BaseScriptNode();

protected:
	UPROPERTY(VisibleAnywhere, Instanced, Category="ScriptGraph")
	UScriptGraphNode* ScriptGraphNode;
	
	SGraphNode_BaseScriptNode* SEdNode;

public:
	void SetScriptGraphNode(UScriptGraphNode* InNode);
	UScriptGraphNode* GetScriptGraphNode() const;

	void SetEdNode(SGraphNode_BaseScriptNode* InNode);
	SGraphNode_BaseScriptNode* GetEdNode() const;

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

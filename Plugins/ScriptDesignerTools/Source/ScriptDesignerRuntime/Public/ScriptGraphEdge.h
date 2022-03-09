// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ScriptGraphEdge.generated.h"

/**
 * 
*/

class UScriptGraph;
class UScriptGraphNode;

UCLASS(Blueprintable)
class SCRIPTDESIGNERRUNTIME_API UScriptGraphEdge : public UObject
{
	GENERATED_BODY()

public :
	UScriptGraphEdge();
	virtual ~UScriptGraphEdge();

protected:
	UPROPERTY(VisibleAnywhere, Category="ScriptGraphEdge")
	UScriptGraph* Graph;

	UPROPERTY(BlueprintReadOnly, Category="ScriptGraphEdge")
	UScriptGraphNode* StartNode;

	UPROPERTY(BlueprintReadOnly, Category="ScriptGraphEdge")
	UScriptGraphNode* EndNode;

public:
	UScriptGraphNode* GetStartNode() const;
	UScriptGraphNode* GetEndNode() const;

	void SetStartNode(UScriptGraphNode* InStartNode);
	void SetEndNode(UScriptGraphNode* InEndNode);
	
	UFUNCTION(BlueprintPure, Category="ScriptGraphEdge")
	UScriptGraph* GetGraph() const;

	void SetGraph(UScriptGraph* InGraph);

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditDefaultsOnly, Category="ScriptGraphEdge_Editor")
	bool bShouldDrawTitle = false;

	UPROPERTY(EditDefaultsOnly, Category="ScriptGraphEdge_Editor")
	FText NodeTitle;

	UPROPERTY(EditDefaultsOnly, Category="ScriptGraphEdge")
	FLinearColor EdgeColor = FLinearColor(0.9f, 0.9f, 0.9f, 1.f);
	
#endif

#if WITH_EDITOR

	virtual FText GetNodeTitle() const { return NodeTitle;}
	FLinearColor GetEdgeColor() { return EdgeColor;}

	virtual void SetNodeTitle(const FText& NewTitle);
	
#endif
	
};

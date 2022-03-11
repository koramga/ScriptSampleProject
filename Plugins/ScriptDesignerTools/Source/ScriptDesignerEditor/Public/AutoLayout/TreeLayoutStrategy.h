// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AutoLayout/AutoLayoutStrategy.h"
#include "TreeLayoutStrategy.generated.h"

/**
 * 
 */
UCLASS()
class SCRIPTDESIGNEREDITOR_API UTreeLayoutStrategy : public UAutoLayoutStrategy
{
	GENERATED_BODY()

public:
	UTreeLayoutStrategy();
	virtual ~UTreeLayoutStrategy();

	virtual void Layout(UEdGraph* InEdGraph) override;

protected:
	void InitPass(UScriptGraphNode* RootNode, const FVector2D& Anchor);
	bool ResolveConflictPass(UScriptGraphNode* Node);

	bool ResolveConflict(UScriptGraphNode* LRoot, UScriptGraphNode* RRoot);

	void GetLeftContour(UScriptGraphNode* RootNode, int32 Level, TArray<UEdNode_ScriptGraphNode*>& Contour);
	void GetRightContour(UScriptGraphNode* RootNode, int32 Level, TArray<UEdNode_ScriptGraphNode*>& Contour);

	void ShiftSubTree(UScriptGraphNode* RootNode, const FVector2D& Offset);

	void UpdateParentNodePosition(UScriptGraphNode* RootNode);
	
};

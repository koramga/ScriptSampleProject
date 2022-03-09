// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ScriptGraphNode.generated.h"

/**
 * 
 */

class UScriptGraph;
class UScriptGraphEdge;

UENUM(BlueprintType)
enum class ENodeLimit : uint8
{
	Unlimited,
	Limited,
};

UCLASS(Blueprintable)
class SCRIPTDESIGNERRUNTIME_API UScriptGraphNode : public UObject
{
	GENERATED_BODY()

public :
	UScriptGraphNode();
	virtual ~UScriptGraphNode();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category="ScriptGraphNode")
	UScriptGraph*	Graph;
	
	UPROPERTY(VisibleDefaultsOnly, Category="ScriptGraphNode")
	TArray<UScriptGraphNode*> ParentNodes;

	UPROPERTY(VisibleDefaultsOnly, Category="ScriptGraphNode")
	TArray<UScriptGraphNode*> ChildrenNodes;

	UPROPERTY(VisibleDefaultsOnly, Category="ScriptGraphNode")
	TMap<UScriptGraphNode*, UScriptGraphEdge*> Edges;

public :
	const TArray<UScriptGraphNode*>& GetChildrenNodes() const;
	void Clear();
	
public :
	UFUNCTION(BlueprintCallable, Category="ScriptGraphNode")
	virtual UScriptGraphEdge* GetEdge(UScriptGraphNode* ChildNode);

	UFUNCTION(BlueprintCallable, Category="ScriptGraphNode")
	bool IsLeafNode() const;

	UFUNCTION(BlueprintCallable, Category="ScriptGraphNode")
	UScriptGraph* GetGraph() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ScriptGraphNode")
	FText GetDescription() const;
	virtual FText GetDescription_Implementation() const;

#if WITH_EDITORONLY_DATA
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="ScriptGraphNode_Editor")
	FText NodeTitle;
	
	UPROPERTY(VisibleDefaultsOnly, Category="ScriptGraphNode_Editor")
	TSubclassOf<UScriptGraphNode> CompatibleGraphType;

	UPROPERTY(EditDefaultsOnly, Category="ScriptGraphNode_Editor")
	FLinearColor BackgroundColor;

	UPROPERTY(EditDefaultsOnly, Category="ScriptGraphNode_Editor")
	FText ContextMenuName;

	UPROPERTY(EditDefaultsOnly, Category="ScriptGraphNode_Editor")
	ENodeLimit ParentLimitType;

	UPROPERTY(EditDefaultsOnly, Category="ScriptGraphNode_Editor", meta = (ClampMin = "0",EditCondition = "ParentLimitType == ENodeLimit::Limited", EditConditionHides))
	int32 ParentLimit;

	UPROPERTY(EditDefaultsOnly, Category="ScriptGraphNode_Editor")
	ENodeLimit ChildrenLimitType;

	UPROPERTY(EditDefaultsOnly, Category="ScriptGraphNode_Editor", meta = (ClampMin = "0",EditCondition = "ParentLimitType == ENodeLimit::Limited", EditConditionHides))
	int32 ChildrenLimit;
	
#endif

#if WITH_EDITOR
public :
	virtual bool IsNameEditable() const;
	virtual FLinearColor GetBackgroundColor() const;
	virtual FText GetNodeTitle() const;
	virtual void SetNodeTitle(const FText& NewTitle);
	virtual bool CanCreateConnection(UScriptGraphNode* Other, FText& ErrorMessage);
	virtual bool CanCreateConnectionTo(UScriptGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage);
	virtual bool CanCreateConnectionFrom(UScriptGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage);
#endif
};

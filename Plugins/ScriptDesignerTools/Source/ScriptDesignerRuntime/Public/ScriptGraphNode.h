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
	TArray<UScriptGraphNode*>& GetChildrenNodes();
	TArray<UScriptGraphNode*>& GetParentNodes();
	void Clear();

	void AddChildNode(UScriptGraphNode* ChildNode);
	void RemoveChildNode(UScriptGraphNode* ChildNode);
	void ClearChildNodes();
	bool IsEmptyChildNode() const;
	void AddParentNode(UScriptGraphNode* ParentNode);
	void RemoveParentNode(UScriptGraphNode* ParentNode);
	void ClearParentNodes();
	bool IsEmptyParentNode() const;
	void AddEdge(UScriptGraphNode* InNode, UScriptGraphEdge* InEdge);
	void SetGraph(UScriptGraph* InGraph);
	void ClearEdges();
	
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

	UPROPERTY(EditDefaultsOnly, Category="ScriptGraphNode_Editor")
	TArray<FString>				BehaviorStrings;
	
#endif

#if WITH_EDITOR
public :
	virtual bool IsNameEditable() const;
	virtual FLinearColor GetBackgroundColor() const;
	virtual FText GetNodeTitle() const;
	virtual FText GetContextMenuName() const;
	virtual TArray<FText> GetBehaviorTexts() const;
	const TSubclassOf<UScriptGraphNode>& GetCompatibleGraphType() const;
	virtual void SetNodeTitle(const FText& NewTitle);
	virtual bool CanCreateConnection(UScriptGraphNode* Other, FText& ErrorMessage);
	virtual bool CanCreateConnectionTo(UScriptGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage);
	virtual bool CanCreateConnectionFrom(UScriptGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage);
#endif
};

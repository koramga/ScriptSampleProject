// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScriptGraphEdge.h"
#include "ScriptGraphNode.h"
#include "UObject/NoExportTypes.h"
#include "ScriptGraph.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SCRIPTDESIGNERRUNTIME_API UScriptGraph : public UObject
{
	GENERATED_BODY()

public :
	UScriptGraph();
	virtual ~UScriptGraph();

protected:
	UPROPERTY(EditDefaultsOnly, Category="ScriptGraph")
	FString Name;

	UPROPERTY(EditDefaultsOnly, Category="ScriptGraph")
	TSubclassOf<UScriptGraphNode>	NodeType;

	UPROPERTY(EditDefaultsOnly, Category="ScriptGraph")
	TSubclassOf<UScriptGraphEdge>	EdgeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ScriptGraph")
	FGameplayTagContainer	GraphTags;

	UPROPERTY(BlueprintReadOnly, Category="ScriptGraph")
	TArray<UScriptGraphNode*> RootNodes;

	UPROPERTY(BlueprintReadOnly, Category="ScriptGraph")
	TArray<UScriptGraphNode*> AllNodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ScriptGraph")
	bool bIsEdgeEnabled;

public :
	FString GetName() const;
	void SetName(const FString& InName);

	bool IsEdgeEnabled() const;
	void SetEdgeEnabled(bool InbIsEdgeEnabled);

	const TArray<UScriptGraphNode*>& GetAllNodes() const;
	void AddNode(UScriptGraphNode* ScriptGraphNode);
	void AddRootNode(UScriptGraphNode* InNode);
	TArray<UScriptGraphNode*>& GetRootNodes();
	
public :
	UFUNCTION(BlueprintCallable, Category="ScriptGraph")
	void Print(bool ToConsole = true, bool ToScreen = true);

	UFUNCTION(BlueprintCallable, Category="ScriptGraph")
	int GetLevelNum() const;

	UFUNCTION(BlueprintCallable, Category="ScriptGraph")
	void GetNodesByLevel(int Level, TArray<UScriptGraphNode*>& Nodes);

	void ClearGraph();

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	class UEdGraph* EdGraph;

	UPROPERTY(EditDefaultsOnly, Category = "ScriptGraph_Editor")
	bool bCanRenameNode;

	UPROPERTY(EditDefaultsOnly, Category = "ScriptGraph_Editor")
	bool bCanBeCyclical;
#endif
};

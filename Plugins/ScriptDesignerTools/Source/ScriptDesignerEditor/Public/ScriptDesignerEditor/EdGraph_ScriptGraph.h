// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph_ScriptGraph.generated.h"

/**
 * 
 */

class UScriptGraph;
class UScriptGraphNode;
class UScriptGraphEdge;
class UEdNode_ScriptGraphEdge;
class UEdNode_ScriptGraphNode;

UCLASS()
class SCRIPTDESIGNEREDITOR_API UEdGraph_ScriptGraph : public UEdGraph
{
	GENERATED_BODY()

public :
	UEdGraph_ScriptGraph();
	virtual ~UEdGraph_ScriptGraph();

	virtual void RebuildGenericGraph();

	UScriptGraph* GetScriptGraph() const;

	virtual bool Modify(bool bAlwaysMarkDirty = true) override;
	virtual void PostEditUndo() override;

protected:
	UPROPERTY(Transient)
	TMap<UScriptGraphNode*, UEdNode_ScriptGraphNode*> NodeMap;

	UPROPERTY(Transient)
	TMap<UScriptGraphEdge*, UEdNode_ScriptGraphEdge*> EdgeMap;

public :
	UEdNode_ScriptGraphNode* FindNode(UScriptGraphNode* Key);
	UEdNode_ScriptGraphEdge* FindEdge(UScriptGraphEdge* Key);	

protected:
	void Clear();
	void SortNodes(UScriptGraphNode* RootNode);
};

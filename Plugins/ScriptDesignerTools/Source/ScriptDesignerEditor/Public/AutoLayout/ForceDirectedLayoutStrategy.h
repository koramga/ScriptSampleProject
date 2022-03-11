// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AutoLayout/AutoLayoutStrategy.h"
#include "ForceDirectedLayoutStrategy.generated.h"

/**
 * 
 */
UCLASS()
class SCRIPTDESIGNEREDITOR_API UForceDirectedLayoutStrategy : public UAutoLayoutStrategy
{
	GENERATED_BODY()
	
public :
	UForceDirectedLayoutStrategy();
	virtual ~UForceDirectedLayoutStrategy();

	virtual void Layout(UEdGraph* InEdGraph) override;

protected :
	virtual FBox2D LayoutOnTree(UScriptGraphNode* RootNode, const FBox2D& PreTreeBound);

protected :
	bool bRandomInit;
	float InitTemperature;
	float CoolDownRate;
};

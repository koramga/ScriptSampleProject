// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoLayout/ForceDirectedLayoutStrategy.h"
#include "ScriptGraph.h"

static inline float CoolDown(float Temp, float CoolDownRate)
{
	if (Temp < .01) return .01;
	return Temp - (Temp / CoolDownRate);
}

static inline float GetAttractForce(float X, float K)
{
	return (X * X) / K;
}

static inline float GetRepulseForce(float X, float k)
{
	return X != 0 ? k * k / X : TNumericLimits<float>::Max();
}

UForceDirectedLayoutStrategy::UForceDirectedLayoutStrategy()
{
	bRandomInit = false;
	CoolDownRate = 10;
	InitTemperature = 10.f;
}

UForceDirectedLayoutStrategy::~UForceDirectedLayoutStrategy()
{

}

void UForceDirectedLayoutStrategy::Layout(UEdGraph* InEdGraph)
{
	EdGraph = Cast<UEdGraph_ScriptGraph>(InEdGraph);
	check(EdGraph != nullptr);

	EdGraph->RebuildGenericGraph();
	Graph = EdGraph->GetScriptGraph();
	check(Graph != nullptr);

	if (Settings != nullptr)
	{
		OptimalDistance = Settings->OptimalDistance;
		MaxIteration = Settings->MaxIteration;
		bRandomInit = Settings->bRandomInit;
	}

	FBox2D PreTreeBound(ForceInitToZero);

	TArray<UScriptGraphNode*>& RootNodes = Graph->GetRootNodes();

	for (UScriptGraphNode* RootNode : RootNodes)
	{
		PreTreeBound = LayoutOnTree(RootNode, PreTreeBound);
	}
}

FBox2D UForceDirectedLayoutStrategy::LayoutOnTree(UScriptGraphNode* RootNode, const FBox2D& PreTreeBound)
{
	float Temp = InitTemperature;
	FBox2D TreeBound = GetActualBounds(RootNode);
	TreeBound.Min.X += PreTreeBound.Max.X + OptimalDistance;
	TreeBound.Max.X += PreTreeBound.Max.X + OptimalDistance;

	if (bRandomInit)
	{
		RandomLayoutOnTree(RootNode, TreeBound);
	}

	float RepulseForce, AttractForce, Distance;
	FVector2D Diff;

	TMap<UEdGraphNode*, FVector2D> NodeToDisplacement;

	for (UEdGraphNode* Node : EdGraph->Nodes)
	{
		NodeToDisplacement.Add(Node, FVector2D(0.f, 0.f));
	}

	for (int32 IterrationNum = 0; IterrationNum < MaxIteration; ++IterrationNum)
	{
		//Calculate the repulsive forces.
		for (UEdGraphNode* MainNode : EdGraph->Nodes)
		{
			for (UEdGraphNode* SubNode : EdGraph->Nodes)
			{
				if (MainNode == SubNode)
				{
					continue;
				}

				Diff.X = MainNode->NodePosX - SubNode->NodePosX;
				Diff.Y = MainNode->NodePosY - SubNode->NodePosY;
				Distance = Diff.Size();
				Diff.Normalize();

				RepulseForce = Distance > 2 * OptimalDistance ? 0 : GetRepulseForce(Distance, OptimalDistance);

				NodeToDisplacement[MainNode] += Diff * RepulseForce;
			}
		}

		//Calculate the attractive forces.
		int Level = 0;
		TArray<UScriptGraphNode*> CurrLevelNodes = { RootNode };
		TArray<UScriptGraphNode*> NextLevelNodes;

		while (CurrLevelNodes.Num() != 0)
		{
			for (UScriptGraphNode* ScriptGraphNode : CurrLevelNodes)
			{
				check(nullptr != ScriptGraphNode);

				UEdGraphNode_BaseScriptNode* EdNode_ParentNode = EdGraph->FindNode(ScriptGraphNode);

				TArray<UScriptGraphNode*>& ChildrenNodes = ScriptGraphNode->GetChildrenNodes();

				for (UScriptGraphNode* ChildNode : ChildrenNodes)
				{
					NextLevelNodes.Add(ChildNode);

					UEdGraphNode_BaseScriptNode* EdNode_ChildNode = EdGraph->FindNode(ChildNode);

					Diff.X = EdNode_ChildNode->NodePosX - EdNode_ParentNode->NodePosY;
					Diff.Y = EdNode_ChildNode->NodePosY - EdNode_ParentNode->NodePosY;
					Distance = Diff.Size();
					Diff.Normalize();

					AttractForce = GetAttractForce(Distance, OptimalDistance);

					NodeToDisplacement[EdNode_ParentNode] += Distance * Diff;
					NodeToDisplacement[EdNode_ChildNode] -= Distance * Diff;
				}
			}

			CurrLevelNodes = NextLevelNodes;
			NextLevelNodes.Reset();
			++Level;
		}

		for (UEdGraphNode* EdNode : EdGraph->Nodes)
		{
			Distance = NodeToDisplacement[EdNode].Size();
			NodeToDisplacement[EdNode].Normalize();

			float Minimum = Distance < Temp ? Distance : Temp;
			EdNode->NodePosX += NodeToDisplacement[EdNode].X * Minimum;
			EdNode->NodePosY += NodeToDisplacement[EdNode].Y * Minimum;
		}

		Temp = CoolDown(Temp, CoolDownRate);
	}

	FBox2D ActualBound = GetActualBounds(RootNode);

	FVector2D Center = ActualBound.GetCenter();
	FVector2D TreeCenter = TreeBound.GetCenter();

	FVector2D Scale = (TreeBound.Max - TreeBound.Min) / (ActualBound.Max - ActualBound.Min);

	for (UEdGraphNode* EdNode : EdGraph->Nodes)
	{
		EdNode->NodePosX = TreeCenter.X + Scale.X * (EdNode->NodePosX - Center.X);
		EdNode->NodePosY = TreeCenter.Y + Scale.Y * (EdNode->NodePosY - Center.Y);
	}

	return TreeBound;
}

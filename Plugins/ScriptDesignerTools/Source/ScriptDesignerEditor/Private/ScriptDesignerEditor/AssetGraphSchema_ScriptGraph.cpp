// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/AssetGraphSchema_ScriptGraph.h"
#include "ToolMenus.h"
#include "ScriptGraphEditorPCH.h"
#include "ScriptDesignerEditor/EdNode_ScriptGraphNode.h"
#include "ScriptDesignerEditor/EdNode_ScriptGraphEdge.h"
#include "ScriptDesignerEditor/ConnectionDrawingPolicy_ScriptGraph.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"
#include "AutoLayout/ForceDirectedLayoutStrategy.h"
#include "AutoLayout/TreeLayoutStrategy.h"

#define LOCTEXT_NAMESPACE "AssetSchema_ScriptGraph"

int32 UAssetGraphSchema_ScriptGraph::CurrentCacheRefreshID = 0;

class FNodeVisitorCycleChecker
{
public :
	/** Check whether a loop in the graph would be caused by linking the passeed-in nodes. */
	bool CheckForLoop(UEdGraphNode* StartNode, UEdGraphNode* EndNode)
	{
		VisitedNodes.Add(StartNode);

		return TraverseNodes(EndNode);
	}

private :
	bool TraverseNodes(UEdGraphNode* Node)
	{
		VisitedNodes.Add(Node);

		for(auto MyPin : Node->Pins)
		{
			if(MyPin->Direction == EGPD_Output)
			{
				for(auto OtherPin : MyPin->LinkedTo)
				{
					UEdGraphNode* OtherNode = OtherPin->GetOwningNode();
					if(VisitedNodes.Contains(OtherNode))
					{
						//Only an issue if this is a back-edge.
						return false;
					}
					else if(!FinishedNodes.Contains(OtherNode))
					{
						//Only should traverse if this node hans't been traversed
						if(!TraverseNodes(OtherNode))
						{
							return false;
						}
					}
				}
			}
		}

		VisitedNodes.Remove(Node);
		FinishedNodes.Add(Node);
		return true;
	}

	TSet<UEdGraphNode*> VisitedNodes;
	TSet<UEdGraphNode*> FinishedNodes;
};

UEdGraphNode* FAssetSchemaAction_ScriptGraph_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;

	if(NodeTemplate != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("ScriptGraphEditorNewNode", "Script Graph Editor : New Node"));
		ParentGraph->Modify();

		if(FromPin != nullptr)
		{
			FromPin->Modify();
		}

		NodeTemplate->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		NodeTemplate->GetScriptGraphNode()->SetFlags(RF_Transactional);
		NodeTemplate->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate;
	}

	return ResultNode;
}

void FAssetSchemaAction_ScriptGraph_NewNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

UEdGraphNode* FAssetSchemaAction_ScriptGraph_NewEdge::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;

	if (NodeTemplate != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("ScriptGraphEditorNewNode", "Script Graph Editor : New Edge"));
		ParentGraph->Modify();
		if (FromPin != nullptr)
			FromPin->Modify();

		NodeTemplate->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		NodeTemplate->GetEdge()->SetFlags(RF_Transactional);
		NodeTemplate->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate;
	}
	
	return ResultNode;
}

void FAssetSchemaAction_ScriptGraph_NewEdge::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

void UAssetGraphSchema_ScriptGraph::GetBreakLinkToSubMenuActions(UToolMenu* Menu, UEdGraphPin* InGraphPin)
{
	//Make sure we have a unique name for every entry in the list.
	TMap<FString, uint32> LinkTitleCount;

	FToolMenuSection& Section = Menu->FindOrAddSection("ScriptGraphAssetGraphSchemaPinAction");

	//Add all the links we could break from
	for(TArray<class UEdGraphPin*>::TConstIterator Links(InGraphPin->LinkedTo); Links; ++Links)
	{
		UEdGraphPin* Pin = *Links;
		FString TitleString = Pin->GetOwningNode()->GetNodeTitle(ENodeTitleType::ListView).ToString();
		FText Title = FText::FromString(TitleString);
		if(Pin->PinName != TEXT(""))
		{
			TitleString = FString::Printf(TEXT("%s (%s"), *TitleString, *Pin->PinName.ToString());

			//Add name of connection if possible
			FFormatNamedArguments Args;
			Args.Add(TEXT("NodeTitle"), Title);
			Args.Add(TEXT("PinName"), Pin->GetDisplayName());
			Title = FText::Format(LOCTEXT("BreakDescPin", "{NodeTitle} ({PinName})"), Args);
		}

		uint32& Count = LinkTitleCount.FindOrAdd(TitleString);

		FText Description;
		FFormatNamedArguments Args;
		Args.Add(TEXT("NodeTitle"), Title);
		Args.Add(TEXT("NumberOfNodes"), Count);

		if(0 == Count)
		{
			Description = FText::Format(LOCTEXT("BreakDesc", "Break line to {NodeTitle}"), Args);
		}
		else
		{
			Description = FText::Format(LOCTEXT("BreakDescMulti", "Break link to {NodeTitle} ({NumberOfNodes})"), Args);
		}
		++Count;

		Section.AddMenuEntry(NAME_None, Description, Description, FSlateIcon(), FUIAction(
			FExecuteAction::CreateUObject(this, &UAssetGraphSchema_ScriptGraph::BreakSinglePinLink, const_cast<UEdGraphPin*>(InGraphPin), *Links)));
	}
}

EGraphType UAssetGraphSchema_ScriptGraph::GetGraphType(const UEdGraph* TestEdGraph) const
{
return GT_StateMachine;
}

void UAssetGraphSchema_ScriptGraph::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	UScriptGraph* Graph = CastChecked<UScriptGraph>(ContextMenuBuilder.CurrentGraph->GetOuter());

	if(false == Graph->IsValidNodeTypeClass())
	{
		return;
	}

	const bool bNoParent = (ContextMenuBuilder.FromPin == NULL);

	const FText AddToolTip = LOCTEXT("NewScriptGraphNodeTooltip", "Add node here");

	TSet<TSubclassOf<UScriptGraphNode>> Visited;

	FText Desc = Graph->GetNodeTypeClass().GetDefaultObject()->GetContextMenuName();

	if(Desc.IsEmpty())
	{
		FString Title = Graph->GetNodeTypeClass()->GetName();
		Title.RemoveFromEnd("_C");
		Desc = FText::FromString(Title);
	}

	if(!Graph->GetNodeTypeClass()->HasAnyClassFlags(CLASS_Abstract))
	{
		TSharedPtr<FAssetSchemaAction_ScriptGraph_NewNode> NewNodeAction(new FAssetSchemaAction_ScriptGraph_NewNode(LOCTEXT("ScriptGraphNodeAction", "Script Graph Node"), Desc, AddToolTip, 0));
		NewNodeAction->NodeTemplate = NewObject<UEdNode_ScriptGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
		NewNodeAction->NodeTemplate->NewScriptGraphNode(NewNodeAction->NodeTemplate, Graph->GetNodeTypeClass());
		NewNodeAction->NodeTemplate->GetScriptGraphNode()->SetGraph(Graph);
		ContextMenuBuilder.AddAction(NewNodeAction);

		Visited.Add(Graph->GetNodeTypeClass());
	}

	for(TObjectIterator<UClass> It; It; ++It)
	{
		if(It->IsChildOf(Graph->GetNodeTypeClass()) && !It->HasAnyClassFlags(CLASS_Abstract) && !Visited.Contains(*It))
		{			
			TSubclassOf<UScriptGraphNode> NodeType = *It;

			if (It->GetName().StartsWith("REINST") || It->GetName().StartsWith("SKEL"))
				continue;

			if (!Graph->GetClass()->IsChildOf(NodeType.GetDefaultObject()->GetCompatibleGraphType()))
				continue;

			Desc = NodeType.GetDefaultObject()->GetContextMenuName();

			if (Desc.IsEmpty())
			{
				FString Title = NodeType->GetName();
				Title.RemoveFromEnd("_C");
				Desc = FText::FromString(Title);
			}

			TSharedPtr<FAssetSchemaAction_ScriptGraph_NewNode> NewNodeAction(new FAssetSchemaAction_ScriptGraph_NewNode(LOCTEXT("ScriptGraphNodeAction", "Script Graph Node"), Desc, AddToolTip, 0));
			NewNodeAction->NodeTemplate = NewObject<UEdNode_ScriptGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
			NewNodeAction->NodeTemplate->NewScriptGraphNode(NewNodeAction->NodeTemplate, NodeType);
			NewNodeAction->NodeTemplate->GetScriptGraphNode()->SetGraph(Graph);
			ContextMenuBuilder.AddAction(NewNodeAction);

			Visited.Add(NodeType);
		}
	}
}

void UAssetGraphSchema_ScriptGraph::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	if(Context->Pin)
	{
		FToolMenuSection& Section = Menu->AddSection("ScriptGraphAssetGraphSchemaNodeActions", LOCTEXT("PinActionsMenuHeader", "Pin Actions"));
		//Only display the 'Break Links' option if there is a link to break!
		if(Context->Pin->LinkedTo.Num() > 0)
		{
			Section.AddMenuEntry(FGraphEditorCommands::Get().BreakPinLinks);

			//add sub menu for break link to
			if(Context->Pin->LinkedTo.Num() > 1)
			{
				Section.AddSubMenu(
					"BreakLineTo",
					LOCTEXT("BreakLinkTo", "Break Link To..."),
					LOCTEXT("BreakSpecificLinks", "Break a specific link..."),
					FNewToolMenuDelegate::CreateUObject((UAssetGraphSchema_ScriptGraph* const)this, &UAssetGraphSchema_ScriptGraph::GetBreakLinkToSubMenuActions, const_cast<UEdGraphPin*>(Context->Pin)));
			}
			else
			{
				((UAssetGraphSchema_ScriptGraph* const)this)->GetBreakLinkToSubMenuActions(Menu, const_cast<UEdGraphPin*>(Context->Pin));
			}
		}
	}
	else if(Context->Node)
	{
		FToolMenuSection& Section = Menu->AddSection("ScriptGraphAssetGraphSchemaNodeActions", LOCTEXT("ClassActionsMenuHeader", "Node Actions"));
		Section.AddMenuEntry(FGenericCommands::Get().Delete);
		Section.AddMenuEntry(FGenericCommands::Get().Cut);
		Section.AddMenuEntry(FGenericCommands::Get().Copy);
		Section.AddMenuEntry(FGenericCommands::Get().Duplicate);

		Section.AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
	}	
	
	Super::GetContextMenuActions(Menu, Context);
}

const FPinConnectionResponse UAssetGraphSchema_ScriptGraph::CanCreateConnection(const UEdGraphPin* A,
	const UEdGraphPin* B) const
{
	//Make sure the pins are not an the same node
	if(A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorSameNode", "Can't connect node to itself"));
	}

	const UEdGraphPin* Out = A;
	const UEdGraphPin* In = B;

	UEdNode_ScriptGraphNode* EdNode_Out = Cast<UEdNode_ScriptGraphNode>(Out->GetOwningNode());
	UEdNode_ScriptGraphNode* EdNode_In = Cast<UEdNode_ScriptGraphNode>(In->GetOwningNode());

	if(EdNode_Out == nullptr || EdNode_In == nullptr)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinError", "Not a valid UScriptGraphEdNode"));
	}

	//Determine if we can have cycles or not
	bool bAllowCycles = false;
	auto EdGraph = Cast<UEdGraph_ScriptGraph>(Out->GetOwningNode()->GetGraph());
	if(EdGraph != nullptr)
	{
		bAllowCycles = EdGraph->GetScriptGraph()->bCanBeCyclical;
	}

	//check for cycles.
	FNodeVisitorCycleChecker CycleChecker;
	if(!bAllowCycles && !CycleChecker.CheckForLoop(Out->GetOwningNode(), In->GetOwningNode()))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorCycle", "Can't create a graph cycle"));
	}

	FText ErrorMessage;
	if(!EdNode_Out->GetScriptGraphNode()->CanCreateConnectionTo(EdNode_In->GetScriptGraphNode(), EdNode_Out->GetOutputPin()->LinkedTo.Num(), ErrorMessage))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, ErrorMessage);
	}
	if(!EdNode_In->GetScriptGraphNode()->CanCreateConnectionFrom(EdNode_Out->GetScriptGraphNode(), EdNode_In->GetInputPin()->LinkedTo.Num(), ErrorMessage))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, ErrorMessage);
	}

	if(EdNode_Out->GetScriptGraphNode()->GetGraph()->IsEdgeEnabled())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE, LOCTEXT("PinConnect", "Connect nodes with edge"));
	}
	else
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, LOCTEXT("PinConnect", "Connect nodes"));
	}
}

bool UAssetGraphSchema_ScriptGraph::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	//We don't actually care about the pin, we want the node that is being dragged between
	UEdNode_ScriptGraphNode* NodeA = Cast<UEdNode_ScriptGraphNode>(A->GetOwningNode());
	UEdNode_ScriptGraphNode* NodeB = Cast<UEdNode_ScriptGraphNode>(B->GetOwningNode());

	//check that this edge doesn't already exist
	for(UEdGraphPin* TestPin : NodeA->GetOutputPin()->LinkedTo)
	{
		UEdGraphNode* ChildNode = TestPin->GetOwningNode();
		if(UEdNode_ScriptGraphEdge* EdNode_Edge = Cast<UEdNode_ScriptGraphEdge>(ChildNode))
		{
			ChildNode = EdNode_Edge->GetEndNode();
		}

		if(ChildNode == NodeB)
		{
			return false;
		}
	}

	if(NodeA && NodeB)
	{
		//Always create connections from node A to B, don't allow adding in reversee
		Super::TryCreateConnection(NodeA->GetOutputPin(), NodeB->GetInputPin());

		return true;
	}

	return false;
}

bool UAssetGraphSchema_ScriptGraph::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const
{
	UEdNode_ScriptGraphNode* NodeA = Cast<UEdNode_ScriptGraphNode>(A->GetOwningNode());
	UEdNode_ScriptGraphNode* NodeB = Cast<UEdNode_ScriptGraphNode>(B->GetOwningNode());

	//Are nodes and pins all valid?
	if(!NodeA || !NodeA->GetOutputPin() || !NodeB || !NodeB->GetInputPin())
	{
		return false;
	}

	UScriptGraph* Graph = NodeA->GetScriptGraphNode()->GetGraph();

	FVector2D InitPos((NodeA->NodePosX + NodeB->NodePosX) / 2, (NodeA->NodePosY + NodeB->NodePosY) / 2);

	FAssetSchemaAction_ScriptGraph_NewEdge Action;
	Action.NodeTemplate = NewObject<UEdNode_ScriptGraphEdge>(NodeA->GetGraph());
	Action.NodeTemplate->SetEdge(NewObject<UScriptGraphEdge>(Action.NodeTemplate, Graph->GetEdgeTypeClass()));
	UEdNode_ScriptGraphEdge* EdgeNode = Cast<UEdNode_ScriptGraphEdge>(Action.PerformAction(NodeA->GetGraph(), nullptr, InitPos, false));

	//Always create connections from node A to B, don't allow adding in reverse
	EdgeNode->CreateConnection(NodeA, NodeB);

	return true;
}

FConnectionDrawingPolicy* UAssetGraphSchema_ScriptGraph::CreateConnectionDrawingPolicy(int32 InBackLayerID,
	int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements,
	UEdGraph* InGraphObj) const
{
	//Line을 그리는 아주 중요한 역할을 수행하는 Policy를 여기서 생성한다.
	return new FConnectionDrawingPolicy_ScriptGraph(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	
	//return Super::CreateConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect,
	//                                            InDrawElements, InGraphObj);
}

FLinearColor UAssetGraphSchema_ScriptGraph::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FColor::White;
}

void UAssetGraphSchema_ScriptGraph::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakNodeLinks", "Break Node Links"));
	
	Super::BreakNodeLinks(TargetNode);
}

void UAssetGraphSchema_ScriptGraph::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakPinLinks", "Break Pin Links"));
	
	Super::BreakPinLinks(TargetPin, bSendsNodeNotifcation);
}

void UAssetGraphSchema_ScriptGraph::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakSinglePinLink", "Break Pin Link"));
	
	Super::BreakSinglePinLink(SourcePin, TargetPin);
}

UEdGraphPin* UAssetGraphSchema_ScriptGraph::DropPinOnNode(UEdGraphNode* InTargetNode, const FName& InSourcePinName,
	const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection) const
{
	UEdNode_ScriptGraphNode* EdNode = Cast<UEdNode_ScriptGraphNode>(InTargetNode);

	switch (InSourcePinDirection)
	{
	case EGPD_Input :
		return EdNode->GetOutputPin();

	case EGPD_Output :
		return EdNode->GetInputPin();

	default :
		return nullptr;
	}
}

bool UAssetGraphSchema_ScriptGraph::SupportsDropPinOnNode(UEdGraphNode* InTargetNode,
	const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection, FText& OutErrorMessage) const
{
	return Cast<UEdNode_ScriptGraphNode>(InTargetNode) != nullptr;
}

bool UAssetGraphSchema_ScriptGraph::IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const
{
	return CurrentCacheRefreshID != InVisualizationCacheID;
}

int32 UAssetGraphSchema_ScriptGraph::GetCurrentVisualizationCacheID() const
{
	return CurrentCacheRefreshID;
}

void UAssetGraphSchema_ScriptGraph::ForceVisualizationCacheClear() const
{
	++CurrentCacheRefreshID;
}

#undef LOCTEXT_NAMESPACE

// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/SEdNode_ScriptGraphEdge.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "ConnectionDrawingPolicy.h"
#include "ScriptGraphEdge.h"
#include "ScriptDesignerEditor/EdNode_ScriptGraphEdge.h"

#define LOCTEXT_NAMESPACE "SScriptGraphEdge"

void SEdNode_ScriptGraphEdge::Construct(const FArguments& InArgs, UEdNode_ScriptGraphEdge* InNode)
{
	this->GraphNode = InNode;
	this->UpdateGraphNode();
}

bool SEdNode_ScriptGraphEdge::RequiresSecondPassLayout() const
{
	return true;
}

void SEdNode_ScriptGraphEdge::PerformSecondPassLayout(
	const TMap<UObject*, TSharedRef<SNode>>& InNodeToWidgetLookup) const
{
	UEdNode_ScriptGraphEdge* EdgeNode = CastChecked<UEdNode_ScriptGraphEdge>(GraphNode);

	FGeometry StartGeom;
	FGeometry EndGeom;

	UEdNode_ScriptGraphNode* Start = EdgeNode->GetStartNode();
	UEdNode_ScriptGraphNode* End = EdgeNode->GetEndNode();
	
	if(nullptr != Start && nullptr != End)
	{
		const TSharedRef<SNode>* FromWidgetPtr = InNodeToWidgetLookup.Find(Start);
		const TSharedRef<SNode>* ToWidgetPtr = InNodeToWidgetLookup.Find(End);

		if(FromWidgetPtr != nullptr
			&& ToWidgetPtr != nullptr)
		{
			const TSharedRef<SNode>& FromWidget = *FromWidgetPtr;
			const TSharedRef<SNode>& ToWidget = *ToWidgetPtr;

			StartGeom = FGeometry(FVector2D(Start->NodePosX, Start->NodePosY), FVector2D::ZeroVector, FromWidget->GetDesiredSize(), 1.f);
			EndGeom = FGeometry(FVector2D(End->NodePosX, End->NodePosY), FVector2D::ZeroVector, ToWidget->GetDesiredSize(), 1.f);
		}
	}

	PositionBetweenTwoNodesWithOffset(StartGeom, EndGeom, 0, 1);
}

void SEdNode_ScriptGraphEdge::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush("Graph.TrasitionNode.ColorSpill"))
				.ColorAndOpacity(this, &SEdNode_ScriptGraphEdge::GetEdgeColor)
			]
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(this, &SEdNode_ScriptGraphEdge::GetEdgeImage)
				.Visibility(this, &SEdNode_ScriptGraphEdge::GetEdgeImageVisibility)
			]

			+ SOverlay::Slot()
			.Padding(FMargin(4.f, 4.f, 4.f, 4.f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SAssignNew(InlineEditableText, SInlineEditableTextBlock)
					.ColorAndOpacity(FLinearColor::Black)
					.Visibility(this, &SEdNode_ScriptGraphEdge::GetEdgeTitleVisibility)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
					.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
					.OnTextCommitted(this, &SEdNode_ScriptGraphEdge::OnNameTextCommited)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					NodeTitle.ToSharedRef()
				]
			]
		];
	
}

void SEdNode_ScriptGraphEdge::PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom,
	int32 NodeIndex, int32 MaxNodes) const
{
	//Get a reasonable seed point (halfway between the boxes)
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;

	//Find the (approximate) closet points between the two boxes.
	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	//Position ourselves halfway along the connecting line between the nodes, elevated away perpendicular to the direction of the line.
	const float Height = 30.f;

	const FVector2D DesiredNodeSize = GetDesiredSize();

	FVector2D DeltaPos(EndAnchorPoint - StartAnchorPoint);

	if(DeltaPos.IsNearlyZero())
	{
		DeltaPos = FVector2D(10.f, 0.f);	
	}

	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();
	const FVector2D NewCenter = StartAnchorPoint + (0.5f * DeltaPos) + (Height * Normal);

	FVector2D DeltaNormal = DeltaPos.GetSafeNormal();

	const float MultiNodeSpace = 0.2f;
	const float MultiNodeStep = 1.f + MultiNodeSpace;

	const float MultiNodeStart = -((MaxNodes - 1) * MultiNodeStep) / 2.f;
	const float MultiNodeOffset = MultiNodeStart + (NodeIndex * MultiNodeStep);

	//Now we need to adjust the new center by the node size, zoom factor and multi node offset
	const FVector2D NewCorner = NewCenter - (0.5f * DesiredNodeSize) + (DeltaNormal * MultiNodeOffset + DesiredNodeSize.Size());

	GraphNode->NodePosX = NewCorner.X;
	GraphNode->NodePosY = NewCorner.Y;
}

void SEdNode_ScriptGraphEdge::OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo)
{
	SGraphNode::OnNameTextCommited(InText, CommitInfo);

	UEdNode_ScriptGraphEdge* MyEdge = CastChecked<UEdNode_ScriptGraphEdge>(GraphNode);

	if(MyEdge != nullptr && MyEdge->GetEdge() != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("ScriptGraphEditorRenameEdge", "Script Graph Editor : Rename Edge"));
		MyEdge->Modify();
		MyEdge->GetEdge()->SetNodeTitle(InText);
		UpdateGraphNode();		
	}
}

FSlateColor SEdNode_ScriptGraphEdge::GetEdgeColor() const
{
	UEdNode_ScriptGraphEdge* EdgeNode = CastChecked<UEdNode_ScriptGraphEdge>(GraphNode);
	if(EdgeNode != nullptr && EdgeNode->GetEdge() != nullptr)
	{
		return EdgeNode->GetEdge()->GetEdgeColor();
	}
	return FLinearColor(0.9f, 0.9f, 0.9f, 1.f);
}

const FSlateBrush* SEdNode_ScriptGraphEdge::GetEdgeImage() const
{
	return FEditorStyle::GetBrush("Graph.TransitionNode.Icon");
}

EVisibility SEdNode_ScriptGraphEdge::GetEdgeImageVisibility() const
{
	UEdNode_ScriptGraphEdge* EdgeNode = CastChecked<UEdNode_ScriptGraphEdge>(GraphNode);
	if(EdgeNode && EdgeNode->GetEdge() && EdgeNode->GetEdge()->bShouldDrawTitle)
	{
		return EVisibility::Hidden;
	}

	return EVisibility::Visible;
}

EVisibility SEdNode_ScriptGraphEdge::GetEdgeTitleVisibility() const
{
	UEdNode_ScriptGraphEdge* EdgeNode = CastChecked<UEdNode_ScriptGraphEdge>(GraphNode);
	
	if (EdgeNode && EdgeNode->GetEdge() && EdgeNode->GetEdge()->bShouldDrawTitle)
		return EVisibility::Visible;

	return EVisibility::Collapsed;
}

#undef LOCTEXT_NAMESPACE
// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/Node/SGraphNode_ScriptNode.h"
#include "GraphEditorSettings.h"
#include "SCommentBubble.h"
#include "ScriptGraph.h"
#include "SGraphPin.h"
#include "ScriptDesignerEditor/Colors_ScriptGraph.h"
#include "ScriptDesignerEditor/ScriptGraphDragConnection.h"
#include "ScriptDesignerEditor/Node/EdNode_ScriptGraphNode.h"
#include "ScriptDesignerEditor/Colors_ScriptGraph.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "ScriptDesignerEditor/Node/EdGraphNode_ScriptNode.h"
#include "ScriptDesignerEditor/Pin/SGraphPin_ScriptPin.h"

#define LOCTEXT_NAMESPACE "EdNode_ScriptGraph"

void SGraphNode_ScriptNode::Construct(const FArguments& InArgs, UEdGraphNode_ScriptNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
	InNode->SetEdNode(this);	
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGraphNode_ScriptNode::UpdateGraphNode()
{
	const FMargin NodePadding = FMargin(5);
	const FMargin NamePadding = FMargin(2);

	InputPins.Empty();
	OutputPins.Empty();

	//Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	const FSlateBrush* NodeTypeIcon = GetNameIcon();

	FLinearColor TitleShadowColor(0.6f, 0.6f, 0.6f);
	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<SVerticalBox> NodeBody;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
			.Padding(0.f)
			.BorderBackgroundColor(this, &SGraphNode_ScriptNode::GetBorderBackgroundColor)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SVerticalBox)

					//Input Pin Area
					+ SVerticalBox::Slot()
					.FillHeight(1)
					[
						SAssignNew(LeftNodeBox, SVerticalBox)
					]

					+ SVerticalBox::Slot()
					.FillHeight(1)
					[
						SAssignNew(RightNodeBox, SVerticalBox)
					]					
				]

				+SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(8.f)
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.ColorSpill"))
					.BorderBackgroundColor(TitleShadowColor)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Visibility(EVisibility::SelfHitTestInvisible)
					.Padding(6.f)
					[
						SAssignNew(NodeBody, SVerticalBox)

						//Title
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SHorizontalBox)

							//Error message
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								SAssignNew(ErrorText, SErrorText)
								.BackgroundColor(this, &SGraphNode_ScriptNode::GetErrorColor)
								.ToolTipText(this, &SGraphNode_ScriptNode::GetErrorMsgToolTip)
							]

							//Icon
							+SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							[
								SNew(SImage)
								.Image(NodeTypeIcon)
							]

							//Node Title
							+ SHorizontalBox::Slot()
							.Padding(FMargin(4.f, 0.f, 4.f, 0.f))
							[
								SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.AutoHeight()
								[
									SAssignNew(InlineEditableText, SInlineEditableTextBlock)
									.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
									.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
									.OnVerifyTextChanged(this, &SGraphNode_ScriptNode::OnVerifyNameTextChanged)
									.OnTextCommitted(this, &SGraphNode_ScriptNode::OnNameTextCommited)
									.IsReadOnly(this, &SGraphNode_ScriptNode::IsNameReadOnly)
									.IsSelected(this, &SGraphNode_ScriptNode::IsSelectedExclusively)
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								[
									NodeTitle.ToSharedRef()
								]
							]
						]
					]
				]
			]
		];

	//Create comment bubble
	TSharedPtr<SCommentBubble> CommentBubble;
	const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

	SAssignNew(CommentBubble, SCommentBubble)
		.GraphNode(GraphNode)
		.Text(this, &SGraphNode::GetNodeComment)
		.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
		.ColorAndOpacity(CommentColor)
		.AllowPinning(true)
		.EnableTitleBarBubble(true)
		.EnableBubbleCtrls(true)
		.GraphLOD(this, &SGraphNode::GetCurrentLOD)
		.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

	GetOrAddSlot(ENodeZone::TopCenter)
		.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
		.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
		.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
		.VAlign(VAlign_Top)
	[
		CommentBubble.ToSharedRef()
	];

	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);
	CreatePinWidgets();
}

void SGraphNode_ScriptNode::CreatePinWidgets()
{	
	//SGraphNode_BaseScriptNode::CreatePinWidgets();
	
	UEdGraphNode_BaseScriptNode* StateNode = CastChecked<UEdGraphNode_BaseScriptNode>(GraphNode);

	for(int32 PinIdx = 0; PinIdx < StateNode->Pins.Num(); ++PinIdx)
	{
		UEdGraphPin* MyPin = StateNode->Pins[PinIdx];
		if(!MyPin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = SNew(SGraphPin_ScriptPin, MyPin);

			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SGraphNode_ScriptNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if(bAdvancedParameter)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}

	TSharedPtr<SVerticalBox> PinBox;
	if(PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		PinBox = LeftNodeBox;
		InputPins.Add(PinToAdd);
	}
	else
	{
		PinBox = RightNodeBox;
		OutputPins.Add(PinToAdd);
	}

	if(PinBox)
	{
		PinBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.f)
			[
				PinToAdd
			];
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE

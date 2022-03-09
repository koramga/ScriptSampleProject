// Fill out your copyright notice in the Description page of Project Settings.
#include "ScriptDesignerEditor/SEdNode_ScriptGraphNode.h"
#include "GraphEditorSettings.h"
#include "SCommentBubble.h"
#include "ScriptGraph.h"
#include "SGraphPin.h"
#include "ScriptDesignerEditor/Colors_ScriptGraph.h"
#include "ScriptDesignerEditor/ScriptGraphDragConnection.h"
#include "ScriptDesignerEditor/EdNode_ScriptGraphNode.h"
#include "ScriptDesignerEditor/Colors_ScriptGraph.h"
#include "SlateOptMacros.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

#define LOCTEXT_NAMESPACE "EdNode_ScriptGraph"

class SScriptGraphPin : public SGraphPin
{
public :
	SLATE_BEGIN_ARGS(SScriptGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		this->SetCursor(EMouseCursor::Default);

		bShowLabel = true;

		GraphPinObj = InPin;
		check(GraphPinObj != nullptr)

		const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
		check(Schema);

		SBorder::Construct(SBorder::FArguments()
			.BorderImage(this, &SScriptGraphPin::GetPinBorder)
			.BorderBackgroundColor(this, &SScriptGraphPin::GetPinColor)
			.OnMouseButtonDown(this, &SScriptGraphPin::OnPinMouseDown)
			.Cursor(this, &SScriptGraphPin::GetPinCursor)
			.Padding(FMargin(5.f))
		);
	}

protected:
	virtual FSlateColor GetPinColor() const override
	{
		return ScriptGraphColor::Pin::Default;
	}

	virtual TSharedRef<SWidget> GetDefaultValueWidget() override
	{
		return SNew(STextBlock);
	}

	const FSlateBrush* GetPinBorder() const
	{
		return FEditorStyle::GetBrush(TEXT("Graph.StateNode.Body"));
	}

	virtual TSharedRef<FDragDropOperation> SpawnPinDragEvent(const TSharedRef<SGraphPanel>& InGraphPanel, const TArray<TSharedRef<SGraphPin>>& InStartingPins) override
	{
		FScriptGraphDragConnection::FDraggedPinTable PinHandles;
		PinHandles.Reserve(InStartingPins.Num());

		//since the graph can be refreshed and pins can be reconstructed/replaced
		//behind the scenes, the DragDropOperation holds onto FGraphPinHandles
		//instead of direct widgets/graph-pins.
		for(const TSharedRef<SGraphPin>& PinWidget : InStartingPins)
		{
			PinHandles.Add(PinWidget->GetPinObj());
		}

		return FScriptGraphDragConnection::New(InGraphPanel, PinHandles);
	}
};

void SEdNode_ScriptGraphNode::Construct(const FArguments& InArgs, UEdNode_ScriptGraphNode* InNode)
{
	//작업 필요
	GraphNode = InNode;
	UpdateGraphNode();
	InNode->SetEdNode(this);
}
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEdNode_ScriptGraphNode::UpdateGraphNode()
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
			.BorderBackgroundColor(this, &SEdNode_ScriptGraphNode::GetBorderBackgroundColor)
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
								.BackgroundColor(this, &SEdNode_ScriptGraphNode::GetErrorColor)
								.ToolTipText(this, &SEdNode_ScriptGraphNode::GetErrorMsgToolTip)
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
									.OnVerifyTextChanged(this, &SEdNode_ScriptGraphNode::OnVerifyNameTextChanged)
									.OnTextCommitted(this, &SEdNode_ScriptGraphNode::OnNameTextCommited)
									.IsReadOnly(this, &SEdNode_ScriptGraphNode::IsNameReadOnly)
									.IsSelected(this, &SEdNode_ScriptGraphNode::IsSelectedExclusively)
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

void SEdNode_ScriptGraphNode::CreatePinWidgets()
{
	UEdNode_ScriptGraphNode* StateNode = CastChecked<UEdNode_ScriptGraphNode>(GraphNode);

	for(int32 PinIdx = 0; PinIdx < StateNode->Pins.Num(); ++PinIdx)
	{
		UEdGraphPin* MyPin = StateNode->Pins[PinIdx];
		if(!MyPin->bHidden)
		{
			TSharedPtr<SGraphPin> NewPin = SNew(SScriptGraphPin, MyPin);

			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SEdNode_ScriptGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
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

bool SEdNode_ScriptGraphNode::IsNameReadOnly() const
{
	UEdNode_ScriptGraphNode* EdNode_Node = Cast<UEdNode_ScriptGraphNode>(GraphNode);
	check(EdNode_Node != nullptr);

	UScriptGraph* ScriptGraph = EdNode_Node->GetScriptGraphNode()->GetGraph();
	check(ScriptGraph != nullptr);

	return (ScriptGraph->bCanRenameNode || !EdNode_Node->GetScriptGraphNode()->IsNameEditable()) || SGraphNode::IsNameReadOnly();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SEdNode_ScriptGraphNode::OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo)
{
	SGraphNode::OnNameTextCommited(InText, CommitInfo);

	UEdNode_ScriptGraphNode* MyNode = CastChecked<UEdNode_ScriptGraphNode>(GraphNode);

	if(MyNode != nullptr && MyNode->GetScriptGraphNode() != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("ScriptGraphEditorRenameNode", "Script Graph Editor : Rename Node"));
		MyNode->Modify();
		MyNode->GetScriptGraphNode()->Modify();
		MyNode->GetScriptGraphNode()->SetNodeTitle(InText);
		UpdateGraphNode();
	}	
}

FSlateColor SEdNode_ScriptGraphNode::GetBorderBackgroundColor() const
{
	UEdNode_ScriptGraphNode* MyNode = CastChecked<UEdNode_ScriptGraphNode>(GraphNode);
	return MyNode ? MyNode->GetBackgroundColor() : ScriptGraphColor::NodeBorder::HighlightAbortRange0;
}

FSlateColor SEdNode_ScriptGraphNode::GetBackgroundColor() const
{
	return ScriptGraphColor::NodeBody::Default;
}

EVisibility SEdNode_ScriptGraphNode::GetDragOverMarkerVisibility() const
{
	return EVisibility::Visible;
}

const FSlateBrush* SEdNode_ScriptGraphNode::GetNameIcon() const
{
	return FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}

#undef LOCTEXT_NAMESPACE
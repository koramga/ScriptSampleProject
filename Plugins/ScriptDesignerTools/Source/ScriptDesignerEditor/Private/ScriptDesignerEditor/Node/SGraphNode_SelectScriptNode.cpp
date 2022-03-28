// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/Node/SGraphNode_SelectScriptNode.h"
#include "EdGraphNode_SelectScriptNode.h"
#include "GraphEditorSettings.h"
#include "IDocumentation.h"
#include "SCommentBubble.h"
#include "SLevelOfDetailBranchNode.h"
#include "TutorialMetaData.h"

void SGraphNode_SelectScriptNode::Construct(const FArguments& InArgs, UEdGraphNode_SelectScriptNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
	InNode->SetEdNode(this);	
}

void SGraphNode_SelectScriptNode::UpdateGraphNode()
{
	SGraphNode::UpdateGraphNode();
}

void SGraphNode_SelectScriptNode::SetDefaultTitleAreaWidget(TSharedRef<SOverlay> DefaultTitleAreaWidget)
{
	DefaultTitleAreaWidget->ClearChildren();
	
	DefaultTitleAreaWidget->AddSlot()
	[
		SNew(SImage)
		.Image( FEditorStyle::GetBrush("Graph.Node.TitleGloss") )
		.ColorAndOpacity( this, &SGraphNode::GetNodeTitleIconColor )
	];

	DefaultTitleAreaWidget->AddSlot()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Center)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SBorder)
			.BorderImage( FEditorStyle::GetBrush("Graph.Node.ColorSpill") )
			.Padding( FMargin(10,5,30,3) )
			.BorderBackgroundColor( this, &SGraphNode::GetNodeTitleColor )
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Hello World"))
			]
		]
	];
}

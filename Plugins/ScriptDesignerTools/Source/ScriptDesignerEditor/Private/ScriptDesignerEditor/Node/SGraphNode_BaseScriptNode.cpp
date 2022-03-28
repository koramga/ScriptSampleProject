// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/Node/SGraphNode_BaseScriptNode.h"

#include "EdGraphNode_BaseScriptNode.h"
#include "ScriptGraph.h"
#include "ScriptDesignerEditor/Colors_ScriptGraph.h"

#define LOCTEXT_NAMESPACE "EdNode_ScriptGraph"

FSlateColor SGraphNode_BaseScriptNode::GetBorderBackgroundColor() const
{
	UEdGraphNode_BaseScriptNode* MyNode = CastChecked<UEdGraphNode_BaseScriptNode>(GraphNode);
	return MyNode ? MyNode->GetBackgroundColor() : ScriptGraphColor::NodeBorder::HighlightAbortRange0;
}

FSlateColor SGraphNode_BaseScriptNode::GetBackgroundColor() const
{
	return ScriptGraphColor::NodeBody::Default;
}

EVisibility SGraphNode_BaseScriptNode::GetDragOverMarkerVisibility() const
{
	return EVisibility::Visible;
}

const FSlateBrush* SGraphNode_BaseScriptNode::GetNameIcon() const
{
	return FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}

void SGraphNode_BaseScriptNode::OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo)
{
	SGraphNode::OnNameTextCommited(InText, CommitInfo);

	UEdGraphNode_BaseScriptNode* MyNode = CastChecked<UEdGraphNode_BaseScriptNode>(GraphNode);

	if(MyNode != nullptr && MyNode->GetScriptGraphNode() != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("ScriptGraphEditorRenameNode", "Script Graph Editor : Rename Node"));
		MyNode->Modify();
		MyNode->GetScriptGraphNode()->Modify();
		MyNode->GetScriptGraphNode()->SetNodeTitle(InText);
		UpdateGraphNode();
	}	
}

bool SGraphNode_BaseScriptNode::IsNameReadOnly() const
{
	UEdGraphNode_BaseScriptNode* EdNode_Node = Cast<UEdGraphNode_BaseScriptNode>(GraphNode);
	check(EdNode_Node != nullptr);

	UScriptGraph* ScriptGraph = EdNode_Node->GetScriptGraphNode()->GetGraph();
	check(ScriptGraph != nullptr);

	return (ScriptGraph->bCanRenameNode || !EdNode_Node->GetScriptGraphNode()->IsNameEditable()) || SGraphNode::IsNameReadOnly();
}

#undef LOCTEXT_NAMESPACE
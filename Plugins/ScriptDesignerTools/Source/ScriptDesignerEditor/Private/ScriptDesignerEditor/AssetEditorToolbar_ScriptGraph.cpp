// Fill out your copyright notice in the Description page of Project Settings.

#include "ScriptDesignerEditor/AssetEditorToolbar_ScriptGraph.h"
#include "ScriptDesignerEditor/AssetEditor_ScriptGraph.h"
#include "ScriptDesignerEditor/EditorCommands_ScriptGraph.h"
#include "ScriptDesignerEditor/ScriptGraphEditorStyle.h"

#define LOCTEXT_NAMESPACE "AssetEditorToolbar_ScriptGraph"


void FAssetEditorToolbar_ScriptGraph::AddScriptGraphToolbar(TSharedPtr<FExtender> Extender)
{
	check(ScriptGraphEditor.IsValid());
	TSharedPtr<FAssetEditor_ScriptGraph> ScriptGraphEditorPtr = ScriptGraphEditor.Pin();
	
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, ScriptGraphEditorPtr->GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP(this, &FAssetEditorToolbar_ScriptGraph::FillScriptGraphToolbar));
	ScriptGraphEditorPtr->AddToolbarExtender(ToolbarExtender);
}

void FAssetEditorToolbar_ScriptGraph::FillScriptGraphToolbar(FToolBarBuilder& ToolBarBuilder)
{
	check(ScriptGraphEditor.IsValid());
	TSharedPtr<FAssetEditor_ScriptGraph> ScriptGraphEditorPtr = ScriptGraphEditor.Pin();

	ToolBarBuilder.BeginSection("Script Graph");
	{
		ToolBarBuilder.AddToolBarButton(FEditorCommands_ScriptGraph::Get().GraphSettings,
			NAME_None,
			LOCTEXT("GraphSettings_Label", "Graph Settings"),
			LOCTEXT("GraphSettings_ToolTip", "Show the Graph Settings"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.GameSettings"));
	}
	ToolBarBuilder.EndSection();

#ifdef ENABLE_AUTO_ARRANGE
	ToolBarBuilder.BeginSection("Util");
	{
		ToolBarBuilder.AddToolBarButton(FEditorCommands_ScriptGraph::Get().AutoArrange,
			NAME_None,
			LOCTEXT("AutoArrange_Label", "AutoArrange"),
			LOCTEXT("AutoArrange_ToolTip", "Auto arrange nodes, not perfect, but still handy"),
			FSlateIcon(FScriptGraphEditorStyle::GetStyleSetName(), "ScriptGraphEditor.AutoArrange"));
	}
	ToolBarBuilder.EndSection();
#endif
}

#undef LOCTEXT_NAMESPACE
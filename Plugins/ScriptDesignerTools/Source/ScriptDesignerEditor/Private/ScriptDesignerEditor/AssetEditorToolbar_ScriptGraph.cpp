// Fill out your copyright notice in the Description page of Project Settings.

#include "ScriptDesignerEditor/AssetEditorToolbar_ScriptGraph.h"
#include "ScriptDesignerEditor/AssetEditor_ScriptGraph.h"
#include "ScriptDesignerEditor/EditorCommands_ScriptGraph.h"
#include "ScriptDesignerEditor/ScriptGraphEditorStyle.h"

#define LOCTEXT_NAMESPACE "AssetEditorToolbar_ScriptGraph"


void FAssetEditorToolbar_ScriptGraph::AddScriptGraphToolbar(TSharedPtr<FExtender> Extender)
{
	//check(ScriptGraphEditor.IsValid());
	//TSharedPtr<FAssetEditor_ScriptGraph> ScriptGraphEditorPtr = ScriptGraphEditor.Pin();
	//
	//TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	//ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, ScriptGraphEditorPtr->GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP(this, &FAssetEditorToolbar_ScriptGraph::FillScriptGraphToolbar));
	//ScriptGraphEditorPtr->
}

void FAssetEditorToolbar_ScriptGraph::FillScriptGraphToolbar(FToolBarBuilder& ToolBarBuilder)
{
}

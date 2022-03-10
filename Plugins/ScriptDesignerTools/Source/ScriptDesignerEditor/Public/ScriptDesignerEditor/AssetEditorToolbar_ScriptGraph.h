// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FAssetEditor_ScriptGraph;
class FExtender;
class FToolBarBuilder;

/**
 * 
 */
class SCRIPTDESIGNEREDITOR_API FAssetEditorToolbar_ScriptGraph : public TSharedFromThis<FAssetEditorToolbar_ScriptGraph>
{
protected :
	/*** Pointer back to the blueprint edtior tool that own us */
	TWeakPtr<FAssetEditor_ScriptGraph> ScriptGraphEditor;

public :
	FAssetEditorToolbar_ScriptGraph(TSharedPtr<FAssetEditor_ScriptGraph> InScriptGraphEditor)
		: ScriptGraphEditor(InScriptGraphEditor) {}

	void AddScriptGraphToolbar(TSharedPtr<FExtender> Extender);

private :
	void FillScriptGraphToolbar(FToolBarBuilder& ToolBarBuilder);
};

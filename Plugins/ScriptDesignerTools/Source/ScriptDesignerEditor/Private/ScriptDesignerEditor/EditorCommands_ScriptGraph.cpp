// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/EditorCommands_ScriptGraph.h"

#define LOCTEXT_NAMESPACE "EditorCommands_ScriptGraph"

void FEditorCommands_ScriptGraph::RegisterCommands()
{
	UI_COMMAND(GraphSettings, "Graph Settings", "Graph Settings", EUserInterfaceActionType::Button, FInputChord());
	//UI_COMMAND(AutoArrange, "Auto Arrange", "Auto Arrange", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE

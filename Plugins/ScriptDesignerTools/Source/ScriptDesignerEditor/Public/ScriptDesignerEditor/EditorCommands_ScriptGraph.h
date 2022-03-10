// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SCRIPTDESIGNEREDITOR_API FEditorCommands_ScriptGraph : public TCommands<FEditorCommands_ScriptGraph>
{
public :
	FEditorCommands_ScriptGraph()
		: TCommands<FEditorCommands_ScriptGraph>("ScriptGraphEditor", NSLOCTEXT("Contxts", "ScriptGraphEditor", "Script Graph Editor"), NAME_None, FEditorStyle::GetStyleSetName())
	{

	}

	TSharedPtr<FUICommandInfo> GraphSettings;
	TSharedPtr<FUICommandInfo> AutoArrange;

	virtual void RegisterCommands() override;
};

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IAssetTools.h"
#include "EdGraphUtilities.h"

class FScriptDesignerEditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private :
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

private :
	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
	EAssetTypeCategories::Type ScriptGraphAssetCategoryBit;
	TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory_ScriptGraph;
};

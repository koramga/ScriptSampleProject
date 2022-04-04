// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScriptDesignerEditor.h"

#include "AssetToolsModule.h"
#include "ScriptDesignerEditor/Private/Factory/ScriptGraphFactory.h"
#include "ScriptDesignerEditor/ScriptGraphEditorStyle.h"
#include "AssetTypeActions_ScriptGraph.h"
#include "Factory/ScriptGraphNodeFactory.h"

#define LOCTEXT_NAMESPACE "FScriptDesignerEditorModule"

void FScriptDesignerEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FScriptGraphEditorStyle::Initialize();

	GraphPanelNodeFactory_ScriptGraph = MakeShareable(new FGraphPanelNodeFactory_ScriptGraph());
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_ScriptGraph);

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	ScriptGraphAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("ScriptGraph")), LOCTEXT("ScriptGraphAssetCategory", "Script"));
	
	RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_ScriptGraph(ScriptGraphAssetCategoryBit)));
}

void FScriptDesignerEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if(FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for(int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
		}
	}

	if(GraphPanelNodeFactory_ScriptGraph.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactory_ScriptGraph);
		GraphPanelNodeFactory_ScriptGraph.Reset();
	}

	FScriptGraphEditorStyle::Shutdown();
}

void FScriptDesignerEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FScriptDesignerEditorModule, ScriptDesignerEditor)
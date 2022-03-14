// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetTypeActions_ScriptGraph.h"
#include "ScriptGraphEditorPCH.h"
#include "ScriptDesignerEditor/AssetEditor_ScriptGraph.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_ScriptGraph"

FAssetTypeActions_ScriptGraph::FAssetTypeActions_ScriptGraph(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FAssetTypeActions_ScriptGraph::GetName() const
{
	return LOCTEXT("FScriptGraphAssetTypeActionsName", "Script Graph");
}

FColor FAssetTypeActions_ScriptGraph::GetTypeColor() const
{
	return FColor(129, 196, 115);
}

UClass* FAssetTypeActions_ScriptGraph::GetSupportedClass() const
{
	return UScriptGraph::StaticClass();
}

void FAssetTypeActions_ScriptGraph::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for(auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if(UScriptGraph* Graph = Cast<UScriptGraph>(*ObjIt))
		{
			TSharedRef<FAssetEditor_ScriptGraph> NewScriptEditor(new FAssetEditor_ScriptGraph());
			NewScriptEditor->InitScriptGraphAssetEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}

uint32 FAssetTypeActions_ScriptGraph::GetCategories()
{
	return MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE
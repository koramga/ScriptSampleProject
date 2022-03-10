// Fill out your copyright notice in the Description page of Project Settings.


#include "AssertTypeAction_ScriptGraph.h"
#include "ScriptGraphEditorPCH.h"

FAssertTypeAction_ScriptGraph::FAssertTypeAction_ScriptGraph(EAssetTypeCategories::Type InAssetCategory)
{
}

FText FAssertTypeAction_ScriptGraph::GetName() const
{
	return FText();
}

FColor FAssertTypeAction_ScriptGraph::GetTypeColor() const
{
	return FColor();
}

UClass* FAssertTypeAction_ScriptGraph::GetSupportedClass() const
{
	return nullptr;
}

void FAssertTypeAction_ScriptGraph::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
}

uint32 FAssertTypeAction_ScriptGraph::GetCategories()
{
	return uint32();
}

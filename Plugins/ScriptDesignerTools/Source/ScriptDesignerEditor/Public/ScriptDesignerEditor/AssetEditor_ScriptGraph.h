// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Settings_ScriptGraphEditor.h"
#include "ScriptGraph.h"

class FGGAssetEditorToolbar;

/**
 * 
 */

//#define ENABLE_AUTO_ARRANGE

class SCRIPTDESIGNEREDITOR_API FAssetEditor_ScriptGraph
	: public FAssetEditorToolkit, public FNotifyHook, public FGCObject 
{
public:
	FAssetEditor_ScriptGraph();
	virtual ~FAssetEditor_ScriptGraph();

	void InitScriptGraphAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UScriptGraph* Graph);

	//IToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	//End of IToolkit interface

	//FAssetEditorToolkit
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FString GetDocumentationLink() const override;
	virtual void SaveAsset_Execute() override;
	//End of FAssetEditorToolkit

	//Toolbar
	void UpdateToolbar();
	TSharedPtr<class FAssetEditorToolbar_ScriptGraph> GetToolbarBuilder() { return ToolbarBuilder; }
	void RegisterToolbarTab(const TSharedRef<class FTabManager>& InTabManager);

	//FSerializableObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	//End of FSerializableObject interface

	UScriptGraphEditorSettings* GetSettings() const;

protected:
	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_EditorSettings(const FSpawnTabArgs& Args);

	void CreateInternalWidgets();
	TSharedRef<SGraphEditor> CreateViewportWidget();

	void BindCommands();
	void CreateEdGraph();
	void CreateCommandList();

	TSharedPtr<SGraphEditor> GetCurrGraphEditor() const;
	FGraphPanelSelectionSet GetSelectedNodes() const;

	void RebuildScriptGraph();

	//Delegates for graph editor commands
	void SelectAllNodes();
	bool CanSelectAllNodes();
	void DeleteSelectedNodes();
	bool CanDeleteNodes();
	void DeleteSelectedDuplicatableNodes();
	void CutSelectedNodes();
	bool CanCutNodes();
	void CopySelectedNodes();
	bool CanCopyNodes();
	void PasteNodes();
	void PasteNodesHere(const FVector2D& Location);
	bool CanPasteNodes();
	void DuplicateNodes();
	bool CanDuplicateNodes();

	void GraphSettings();
	bool CanGraphSettings() const;

	void AutoArrange();
	bool CanAutoArrange() const;

	void OnRenameNode();
	bool CanRenameNodes() const;

	//Graph editor event
	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);
	void OnNodeDoubleClicked(UEdGraphNode* Node);
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);
	void OnPackageSaved(const FString& PackageFileName, UObject* Outer);

protected:
	UScriptGraphEditorSettings* ScriptGraphEditorSettings;
	UScriptGraph* EditingGraph;

	//Toolbar
	TSharedPtr<class FAssetEditorToolbar_ScriptGraph> ToolbarBuilder;

	/** Handle to the registered onpackagesae delegage*/
	FDelegateHandle OnPackageSavedDelegateHandle;

	TSharedPtr<SGraphEditor> ViewportWidget;
	TSharedPtr<class IDetailsView> PropertyWidget;
	TSharedPtr<class IDetailsView> EditorSettingWidget;

	/** The command list for this editor */
	TSharedPtr<FUICommandList> GraphEditorCommands;
	
};

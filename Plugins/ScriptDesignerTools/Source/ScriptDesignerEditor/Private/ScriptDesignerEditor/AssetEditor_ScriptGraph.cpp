// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/AssetEditor_ScriptGraph.h"

#include "GraphEditorActions.h"
#include "ScriptGraphEditorPCH.h"
#include "AutoLayout/ForceDirectedLayoutStrategy.h"
#include "AutoLayout/TreeLayoutStrategy.h"
#include "Framework/Commands/GenericCommands.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "HAL/PlatformApplicationMisc.h"
#include "ScriptDesignerEditor/AssetEditorToolbar_ScriptGraph.h"
#include "ScriptDesignerEditor/AssetGraphSchema_ScriptGraph.h"
#include "ScriptDesignerEditor/EdGraph_ScriptGraph.h"
#include "ScriptDesignerEditor/EditorCommands_ScriptGraph.h"
#include "ScriptDesignerEditor/EdNode_ScriptGraphEdge.h"
#include "ScriptDesignerEditor/EdNode_ScriptGraphNode.h"


#define LOCTEXT_NAMESPACE "AssetEditor_ScriptGraph"

const FName ScriptGraphEditorAppName = FName(TEXT("ScriptGraphEditorApp"));

struct FScriptGraphAssetEditorTabs
{
	//Tab idnetifiers
	static const FName ScriptGraphPropertyID;
	static const FName ViewpoertID;
	static const FName ScriptGraphEditorSettingsID;
};

const FName FScriptGraphAssetEditorTabs::ScriptGraphPropertyID(TEXT("ScriptGraphProperty"));
const FName FScriptGraphAssetEditorTabs::ViewpoertID(TEXT("Viewport"));
const FName FScriptGraphAssetEditorTabs::ScriptGraphEditorSettingsID(TEXT("ScriptrGraphEditorSettings"));

FAssetEditor_ScriptGraph::FAssetEditor_ScriptGraph()
{
	EditingGraph = nullptr;

	ScriptGraphEditorSettings = NewObject<UScriptGraphEditorSettings>(UScriptGraphEditorSettings::StaticClass());

	OnPackageSavedDelegateHandle = UPackage::PackageSavedEvent.AddRaw(this, &FAssetEditor_ScriptGraph::OnPackageSaved);
}

FAssetEditor_ScriptGraph::~FAssetEditor_ScriptGraph()
{
	UPackage::PackageSavedEvent.Remove(OnPackageSavedDelegateHandle);
}

void FAssetEditor_ScriptGraph::InitScriptGraphAssetEditor(const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost, UScriptGraph* Graph)
{
	EditingGraph = Graph;
	CreateEdGraph();

	FGenericCommands::Register();
	FGraphEditorCommands::Register();
	FEditorCommands_ScriptGraph::Register();

	if(!ToolbarBuilder.IsValid())
	{
		ToolbarBuilder = MakeShareable(new FAssetEditorToolbar_ScriptGraph(SharedThis(this)));
	}

	BindCommands();

	CreateInternalWidgets();

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	ToolbarBuilder->AddScriptGraphToolbar(ToolbarExtender);

	//Layout
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_ScriptGraphEditor_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)->SetHideTabWell(true)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.65f)
					->AddTab(FScriptGraphAssetEditorTabs::ViewpoertID, ETabState::OpenedTab)->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.7f)
						->AddTab(FScriptGraphAssetEditorTabs::ScriptGraphPropertyID, ETabState::OpenedTab)->SetHideTabWell(true)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.3f)
						->AddTab(FScriptGraphAssetEditorTabs::ScriptGraphEditorSettingsID, ETabState::OpenedTab)
					)
				)
			)			
		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, ScriptGraphEditorAppName, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, EditingGraph, false);

	RegenerateMenusAndToolbars();
}

void FAssetEditor_ScriptGraph::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_ScriptGraphEditor", "Script Graph Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FScriptGraphAssetEditorTabs::ViewpoertID, FOnSpawnTab::CreateSP(this, &FAssetEditor_ScriptGraph::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("GraphCanvasTabl", "Viewpoert"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor_EventGraph_16x"));

	InTabManager->RegisterTabSpawner(FScriptGraphAssetEditorTabs::ScriptGraphPropertyID, FOnSpawnTab::CreateSP(this, &FAssetEditor_ScriptGraph::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTab", "Property"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(FScriptGraphAssetEditorTabs::ScriptGraphEditorSettingsID, FOnSpawnTab::CreateSP(this, &FAssetEditor_ScriptGraph::SpawnTab_EditorSettings))
		.SetDisplayName(LOCTEXT("EditorSettingsTab", "Script Graph Editor Settings"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FAssetEditor_ScriptGraph::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FScriptGraphAssetEditorTabs::ViewpoertID);
	InTabManager->UnregisterTabSpawner(FScriptGraphAssetEditorTabs::ScriptGraphPropertyID);
	InTabManager->UnregisterTabSpawner(FScriptGraphAssetEditorTabs::ScriptGraphEditorSettingsID);
}

FName FAssetEditor_ScriptGraph::GetToolkitFName() const
{
	return FName("FScriptGraphEditor");
}

FText FAssetEditor_ScriptGraph::GetBaseToolkitName() const
{
	return LOCTEXT("ScriptGraphEditorAppLabel", "Script Graph Editor");
}

FText FAssetEditor_ScriptGraph::GetToolkitName() const
{
	const bool bDirtyState = EditingGraph->GetOutermost()->IsDirty();

	FFormatNamedArguments Args;
	Args.Add(TEXT("ScriptGraphName"), FText::FromString(EditingGraph->GetName()));
	Args.Add(TEXT("DirtyState"), bDirtyState ? FText::FromString(TEXT("*")) : FText::GetEmpty());
	
	return FText::Format(LOCTEXT("ScriptGraphEditorToolkitName", "{ScriptGraphName}{DirtyState}"), Args);
}

FText FAssetEditor_ScriptGraph::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(EditingGraph);
}

FLinearColor FAssetEditor_ScriptGraph::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

FString FAssetEditor_ScriptGraph::GetWorldCentricTabPrefix() const
{
	return TEXT("ScriptGraphEditor");
}

FString FAssetEditor_ScriptGraph::GetDocumentationLink() const
{
	return TEXT("");
}

void FAssetEditor_ScriptGraph::SaveAsset_Execute()
{
	if(EditingGraph != nullptr)
	{
		RebuildScriptGraph();
	}

	FAssetEditorToolkit::SaveAsset_Execute();
}

void FAssetEditor_ScriptGraph::UpdateToolbar()
{
	
}

void FAssetEditor_ScriptGraph::RegisterToolbarTab(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FAssetEditor_ScriptGraph::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditingGraph);
	Collector.AddReferencedObject(EditingGraph->EdGraph);
}

UScriptGraphEditorSettings* FAssetEditor_ScriptGraph::GetSettings() const
{
	return ScriptGraphEditorSettings;
}

TSharedRef<SDockTab> FAssetEditor_ScriptGraph::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FScriptGraphAssetEditorTabs::ViewpoertID)

	TSharedRef<SDockTab> SpawnTab = SNew(SDockTab)
		.Label(LOCTEXT("ViewportTab_Title", "Viewport"));

	if(ViewportWidget.IsValid())
	{
		SpawnTab->SetContent(ViewportWidget.ToSharedRef());
	}

	return SpawnTab;
}

TSharedRef<SDockTab> FAssetEditor_ScriptGraph::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FScriptGraphAssetEditorTabs::ScriptGraphPropertyID);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("Details_Title", "Property"))
		[
			PropertyWidget.ToSharedRef()
		];
}

TSharedRef<SDockTab> FAssetEditor_ScriptGraph::SpawnTab_EditorSettings(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FScriptGraphAssetEditorTabs::ScriptGraphEditorSettingsID);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("EditorSettings_Title", "Generic Graph Editor Setttings"))
		[
			EditorSettingWidget.ToSharedRef()
		];
}

void FAssetEditor_ScriptGraph::CreateInternalWidgets()
{
	ViewportWidget = CreateViewportWidget();

	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyWidget = PropertyModule.CreateDetailView(Args);
	PropertyWidget->SetObject(EditingGraph);
	PropertyWidget->OnFinishedChangingProperties().AddSP(this, &FAssetEditor_ScriptGraph::OnFinishedChangingProperties);

	EditorSettingWidget = PropertyModule.CreateDetailView(Args);
	EditorSettingWidget->SetObject(ScriptGraphEditorSettings);
}

TSharedRef<SGraphEditor> FAssetEditor_ScriptGraph::CreateViewportWidget()
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_ScriptGraph", "Script Graph");

	CreateCommandList();

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FAssetEditor_ScriptGraph::OnSelectedNodesChanged);
	InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FAssetEditor_ScriptGraph::OnNodeDoubleClicked);

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(EditingGraph->EdGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}

void FAssetEditor_ScriptGraph::BindCommands()
{
	ToolkitCommands->MapAction(FEditorCommands_ScriptGraph::Get().GraphSettings,
		FExecuteAction::CreateSP(this, &FAssetEditor_ScriptGraph::GraphSettings),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_ScriptGraph::CanGraphSettings)
	);

	ToolkitCommands->MapAction(FEditorCommands_ScriptGraph::Get().AutoArrange,
		FExecuteAction::CreateSP(this, &FAssetEditor_ScriptGraph::AutoArrange),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_ScriptGraph::CanAutoArrange)
	);
}

void FAssetEditor_ScriptGraph::CreateEdGraph()
{
	if(EditingGraph->EdGraph == nullptr)
	{
		EditingGraph->EdGraph = CastChecked<UEdGraph_ScriptGraph>(FBlueprintEditorUtils::CreateNewGraph(EditingGraph, NAME_None, UEdGraph_ScriptGraph::StaticClass(), UAssetGraphSchema_ScriptGraph::StaticClass()));
		EditingGraph->EdGraph->bAllowDeletion = false;

		//Give the schema a chance to fill out any required nodes (like the results node)
		const UEdGraphSchema* Schema = EditingGraph->EdGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*EditingGraph->EdGraph);
	}
}

void FAssetEditor_ScriptGraph::CreateCommandList()
{
	if(GraphEditorCommands.IsValid())
	{
		return;
	}

	GraphEditorCommands = MakeShareable(new FUICommandList);

	//Can't use CreateSP here because derived editor are already implementing TSharedFromThis<FAssetEditorToolKit>
	//however it should be safe, since commands are being used only within this editor
	//if it ever crashes, this function will have to go away and be reimplemented in each derived class

	GraphEditorCommands->MapAction(FEditorCommands_ScriptGraph::Get().GraphSettings,
			FExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::GraphSettings),
			FCanExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::CanGraphSettings));

	GraphEditorCommands->MapAction(FEditorCommands_ScriptGraph::Get().AutoArrange,
			FExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::AutoArrange),
			FCanExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::CanAutoArrange));

	GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
		FExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::SelectAllNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::CanSelectAllNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::CanDeleteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::CanCopyNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
		FExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::CanCutNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
		FExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::PasteNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::CanPasteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
		FExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::DuplicateNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_ScriptGraph::CanDuplicateNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Rename,
		FExecuteAction::CreateSP(this, &FAssetEditor_ScriptGraph::OnRenameNode),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_ScriptGraph::CanRenameNodes)
	);
}

TSharedPtr<SGraphEditor> FAssetEditor_ScriptGraph::GetCurrGraphEditor() const
{
	return ViewportWidget;
}

FGraphPanelSelectionSet FAssetEditor_ScriptGraph::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelection;
	TSharedPtr<SGraphEditor> FocusedGraphEd = GetCurrGraphEditor();

	if(FocusedGraphEd.IsValid())
	{
		CurrentSelection = FocusedGraphEd->GetSelectedNodes();
	}

	return CurrentSelection;
}

void FAssetEditor_ScriptGraph::RebuildScriptGraph()
{
	if(EditingGraph == nullptr)
	{
		LOG_WARNING(TEXT("FGenericGraphAssetEditor::RebuildGenericGraph EditingGraph is nullptr"));
		return;
	}

	UEdGraph_ScriptGraph* EdGraph = Cast<UEdGraph_ScriptGraph>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	EdGraph->RebuildGenericGraph();
}

void FAssetEditor_ScriptGraph::SelectAllNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		CurrentGraphEditor->SelectAllNodes();
	}	
}

bool FAssetEditor_ScriptGraph::CanSelectAllNodes()
{
	return true;
}

void FAssetEditor_ScriptGraph::DeleteSelectedNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());

	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* EdNode = Cast<UEdGraphNode>(*NodeIt);
		if (EdNode == nullptr || !EdNode->CanUserDeleteNode())
			continue;;

		if (UEdNode_ScriptGraphNode* EdNode_Node = Cast<UEdNode_ScriptGraphNode>(EdNode))
		{
			EdNode_Node->Modify();

			const UEdGraphSchema* Schema = EdNode_Node->GetSchema();
			if (Schema != nullptr)
			{
				Schema->BreakNodeLinks(*EdNode_Node);
			}

			EdNode_Node->DestroyNode();
		}
		else
		{
			EdNode->Modify();
			EdNode->DestroyNode();
		}
	}
}

bool FAssetEditor_ScriptGraph::CanDeleteNodes()
{
	//If any of the nodes can be deleted then we should allow deleting
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	for(FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if(Node != nullptr && Node->CanUserDeleteNode())
		{
			return true;
		}
	}

	return false;
}

void FAssetEditor_ScriptGraph::DeleteSelectedDuplicatableNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();

	if(CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FGraphPanelSelectionSet OldSelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for(FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if(Node && Node->CanDuplicateNode())
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}

	//Delete the duplicatable nodes
	DeleteSelectedNodes();

	CurrentGraphEditor->ClearSelectionSet();

	for(FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if(UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
}

void FAssetEditor_ScriptGraph::CutSelectedNodes()
{
	CopySelectedNodes();
	DeleteSelectedDuplicatableNodes();
}

bool FAssetEditor_ScriptGraph::CanCutNodes()
{
	return CanCopyNodes() && CanDeleteNodes();
}

void FAssetEditor_ScriptGraph::CopySelectedNodes()
{
	//Export the selected nodes and place the text on the clipboard
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	FString ExportedText;

	for(FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if(Node == nullptr)
		{
			SelectedIter.RemoveCurrent();
			continue;;
		}

		if(UEdNode_ScriptGraphEdge* EdNode_Edge = Cast<UEdNode_ScriptGraphEdge>(*SelectedIter))
		{
			UEdNode_ScriptGraphNode* StartNode = EdNode_Edge->GetStartNode();
			UEdNode_ScriptGraphNode* EndNode = EdNode_Edge->GetEndNode();

			if(!SelectedNodes.Contains(StartNode) || !SelectedNodes.Contains(EndNode))
			{
				SelectedIter.RemoveCurrent();
				continue;
			}
		}

		Node->PrepareForCopying();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FAssetEditor_ScriptGraph::CanCopyNodes()
{
	// If any of the nodes can be duplicated then we should allow copying
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}

	return false;
}

void FAssetEditor_ScriptGraph::PasteNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		PasteNodesHere(CurrentGraphEditor->GetPasteLocation());
	}
}

void FAssetEditor_ScriptGraph::PasteNodesHere(const FVector2D& Location)
{
	// Find the graph editor with focus
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}
	// Select the newly pasted stuff
	UEdGraph* EdGraph = CurrentGraphEditor->GetCurrentGraph();

	{
		const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
		EdGraph->Modify();

		// Clear the selection set (newly pasted stuff will be selected)
		CurrentGraphEditor->ClearSelectionSet();

		// Grab the text to paste from the clipboard.
		FString TextToImport;
		FPlatformApplicationMisc::ClipboardPaste(TextToImport);

		// Import the nodes
		TSet<UEdGraphNode*> PastedNodes;
		FEdGraphUtilities::ImportNodesFromText(EdGraph, TextToImport, PastedNodes);

		//Average position of nodes so we can move them while still maintaining relative distances to each other
		FVector2D AvgNodePosition(0.0f, 0.0f);

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			UEdGraphNode* Node = *It;
			AvgNodePosition.X += Node->NodePosX;
			AvgNodePosition.Y += Node->NodePosY;
		}

		float InvNumNodes = 1.0f / float(PastedNodes.Num());
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			UEdGraphNode* Node = *It;
			CurrentGraphEditor->SetNodeSelection(Node, true);

			Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X;
			Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y;

			Node->SnapToGrid(16);

			// Give new node a different Guid from the old one
			Node->CreateNewGuid();
		}
	}

	// Update UI
	CurrentGraphEditor->NotifyGraphChanged();

	UObject* GraphOwner = EdGraph->GetOuter();
	if (GraphOwner)
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}
}

bool FAssetEditor_ScriptGraph::CanPasteNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return false;
	}

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(CurrentGraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FAssetEditor_ScriptGraph::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool FAssetEditor_ScriptGraph::CanDuplicateNodes()
{
	return CanCopyNodes();
}

void FAssetEditor_ScriptGraph::GraphSettings()
{
	PropertyWidget->SetObject(EditingGraph);
}

bool FAssetEditor_ScriptGraph::CanGraphSettings() const
{
	return true;
}

void FAssetEditor_ScriptGraph::AutoArrange()
{
	UEdGraph_ScriptGraph* EdGraph = Cast<UEdGraph_ScriptGraph>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	const FScopedTransaction Transaction(LOCTEXT("GenericGraphEditorAutoArrange", "Generic Graph Editor: Auto Arrange"));

	EdGraph->Modify();

	UAutoLayoutStrategy* LayoutStrategy = nullptr;
	switch (ScriptGraphEditorSettings->AutoLayoutStrategy)
	{
	case EAutoLayoutStrategy::Tree:
		LayoutStrategy = NewObject<UAutoLayoutStrategy>(EdGraph, UTreeLayoutStrategy::StaticClass());
		break;
	case EAutoLayoutStrategy::ForceDirected:
		LayoutStrategy = NewObject<UAutoLayoutStrategy>(EdGraph, UForceDirectedLayoutStrategy::StaticClass());
		break;
	default:
		break;
	}

	if (LayoutStrategy != nullptr)
	{
		LayoutStrategy->Settings = ScriptGraphEditorSettings;
		LayoutStrategy->Layout(EdGraph);
		LayoutStrategy->ConditionalBeginDestroy();
	}
	else
	{
		LOG_ERROR(TEXT("FAssetEditor_GenericGraph::AutoArrange LayoutStrategy is null."));
	}
}

bool FAssetEditor_ScriptGraph::CanAutoArrange() const
{
	return EditingGraph != nullptr && Cast<UEdGraph_ScriptGraph>(EditingGraph->EdGraph) != nullptr;
}

void FAssetEditor_ScriptGraph::OnRenameNode()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			UEdGraphNode* SelectedNode = Cast<UEdGraphNode>(*NodeIt);
			if (SelectedNode != NULL && SelectedNode->bCanRenameNode)
			{
				CurrentGraphEditor->IsNodeTitleVisible(SelectedNode, true);
				break;
			}
		}
	}
}

bool FAssetEditor_ScriptGraph::CanRenameNodes() const
{
	UEdGraph_ScriptGraph* EdGraph = Cast<UEdGraph_ScriptGraph>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	UScriptGraph* Graph = EdGraph->GetScriptGraph();
	check(Graph != nullptr)

	return Graph->bCanRenameNode && GetSelectedNodes().Num() == 1;
}

void FAssetEditor_ScriptGraph::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	TArray<UObject*> Selection;

	for (UObject* SelectionEntry : NewSelection)
	{
		Selection.Add(SelectionEntry);
	}

	if (Selection.Num() == 0) 
	{
		PropertyWidget->SetObject(EditingGraph);

	}
	else
	{
		PropertyWidget->SetObjects(Selection);
	}
}

void FAssetEditor_ScriptGraph::OnNodeDoubleClicked(UEdGraphNode* Node)
{
}

void FAssetEditor_ScriptGraph::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (EditingGraph == nullptr)
		return;

	EditingGraph->EdGraph->GetSchema()->ForceVisualizationCacheClear();
}

void FAssetEditor_ScriptGraph::OnPackageSaved(const FString& PackageFileName, UObject* Outer)
{
	RebuildScriptGraph();
}

#undef LOCTEXT_NAMESPACE
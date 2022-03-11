// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptDesignerEditor/AssetEditor_ScriptGraph.h"

#include "GraphEditorActions.h"
#include "ScriptGraphEditorPCH.h"
#include "Framework/Commands/GenericCommands.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "ScriptDesignerEditor/AssetEditorToolbar_ScriptGraph.h"
#include "ScriptDesignerEditor/EdGraph_ScriptGraph.h"
#include "ScriptDesignerEditor/EditorCommands_ScriptGraph.h"


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
	//if(EditingGraph->EdGraph == nullptr)
	//{
	//	EditingGraph->EdGraph = CastChecked<UEdGraph_ScriptGraph>(FBlueprintEditorUtils::CreateNewGraph(EditingGraph, NAME_None, UEdGraph_ScriptGraph::StaticClass(), UAssetGraphSc))
	//}
}

void FAssetEditor_ScriptGraph::CreateCommandList()
{
}

TSharedPtr<SGraphEditor> FAssetEditor_ScriptGraph::GetCurrGraphEditor() const
{
	return nullptr;
}

FGraphPanelSelectionSet FAssetEditor_ScriptGraph::GetSelectedNodes() const
{
	return FGraphPanelSelectionSet();
}

void FAssetEditor_ScriptGraph::RebuildScriptGraph()
{
}

void FAssetEditor_ScriptGraph::SelectAllNodes()
{
}

bool FAssetEditor_ScriptGraph::CanSelectAllNodes()
{
	return false;
}

void FAssetEditor_ScriptGraph::DeleteSelectedNodes()
{
}

bool FAssetEditor_ScriptGraph::CanDeleteNodes()
{
	return false;
}

void FAssetEditor_ScriptGraph::DeleteSelectedDuplicatableNodes()
{
}

void FAssetEditor_ScriptGraph::CutSelectedNodes()
{
}

bool FAssetEditor_ScriptGraph::CanCutNodes()
{
	return false;
}

void FAssetEditor_ScriptGraph::CopySelectedNodes()
{
}

bool FAssetEditor_ScriptGraph::CanCopyNodes()
{
	return false;
}

void FAssetEditor_ScriptGraph::PasteNodes()
{
}

void FAssetEditor_ScriptGraph::PasteNodesHere(const FVector2D& Location)
{
}

bool FAssetEditor_ScriptGraph::CanPasteNodes()
{
	return false;
}

void FAssetEditor_ScriptGraph::DuplicateNodes()
{
}

bool FAssetEditor_ScriptGraph::CanDuplicateNodes()
{
	return false;
}

void FAssetEditor_ScriptGraph::GraphSettings()
{
}

bool FAssetEditor_ScriptGraph::CanGraphSettings() const
{
	return false;
}

void FAssetEditor_ScriptGraph::AutoArrange()
{
}

bool FAssetEditor_ScriptGraph::CanAutoArrange() const
{
	return false;
}

void FAssetEditor_ScriptGraph::OnRenameNode()
{
}

bool FAssetEditor_ScriptGraph::CanRenameNodes() const
{
	return false;
}

void FAssetEditor_ScriptGraph::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
}

void FAssetEditor_ScriptGraph::OnNodeDoubleClicked(UEdGraphNode* Node)
{
}

void FAssetEditor_ScriptGraph::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
}

void FAssetEditor_ScriptGraph::OnPackageSaved(const FString& PackageFileName, UObject* Outer)
{
}

#undef LOCTEXT_NAMESPACE
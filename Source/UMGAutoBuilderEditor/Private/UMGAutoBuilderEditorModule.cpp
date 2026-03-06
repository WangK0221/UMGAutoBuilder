#include "UMGAutoBuilderEditorModule.h"

#include "Framework/Docking/TabManager.h"
#include "Modules/ModuleManager.h"
#include "ToolMenus.h"
#include "UMGAutoBuilderService.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

namespace
{
	static const FName UMGAutoBuilderTabName(TEXT("UMGAutoBuilderWorkbench"));

	static FString FormatReport(const FUMGAutoBuilderReport& Report, const bool bSuccess)
	{
		FString Output;
		Output += bSuccess ? TEXT("[Success]\n") : TEXT("[Failed]\n");
		if (!Report.Summary.IsEmpty())
		{
			Output += Report.Summary + TEXT("\n");
		}

		if (Report.Warnings.Num() > 0)
		{
			Output += TEXT("\nWarnings:\n");
			for (const FString& Warning : Report.Warnings)
			{
				Output += TEXT("- ") + Warning + TEXT("\n");
			}
		}

		if (Report.Errors.Num() > 0)
		{
			Output += TEXT("\nErrors:\n");
			for (const FString& Error : Report.Errors)
			{
				Output += TEXT("- ") + Error + TEXT("\n");
			}
		}

		return Output;
	}

	class SUMGAutoBuilderWorkbench final : public SCompoundWidget
	{
	public:
		SLATE_BEGIN_ARGS(SUMGAutoBuilderWorkbench)
		{
		}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs)
		{
			ChildSlot
			[
				SNew(SBorder)
				.Padding(10.0f)
				[
					SNew(SScrollBox)
					+ SScrollBox::Slot()
					[
						SNew(SVerticalBox)

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("UMG Auto Builder Workbench")))
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 8.0f, 0.0f, 2.0f)
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Config Path")))
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(ConfigPathTextBox, SEditableTextBox)
							.HintText(FText::FromString(TEXT("Path to JSON config used by build/patch")))
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 8.0f, 0.0f, 0.0f)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.Padding(0.0f, 0.0f, 8.0f, 0.0f)
							[
								SNew(SButton)
								.Text(FText::FromString(TEXT("Run Build")))
								.OnClicked(this, &SUMGAutoBuilderWorkbench::OnBuildClicked)
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								SNew(SButton)
								.Text(FText::FromString(TEXT("Run Patch")))
								.OnClicked(this, &SUMGAutoBuilderWorkbench::OnPatchClicked)
							]
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 12.0f, 0.0f, 12.0f)
						[
							SNew(SSeparator)
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Widget Path")))
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 2.0f, 0.0f, 0.0f)
						[
							SAssignNew(WidgetPathTextBox, SEditableTextBox)
							.HintText(FText::FromString(TEXT("/Game/UI/WB_Example.WB_Example")))
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 8.0f, 0.0f, 2.0f)
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Export Output Path")))
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(ExportPathTextBox, SEditableTextBox)
							.HintText(FText::FromString(TEXT("Optional .json output path")))
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 8.0f, 0.0f, 0.0f)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							.Padding(0.0f, 0.0f, 8.0f, 0.0f)
							[
								SNew(SCheckBox)
								.IsChecked(this, &SUMGAutoBuilderWorkbench::GetPrettyExportCheckState)
								.OnCheckStateChanged(this, &SUMGAutoBuilderWorkbench::OnPrettyExportChanged)
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("Pretty Export")))
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.Padding(16.0f, 0.0f, 0.0f, 0.0f)
							[
								SNew(SButton)
								.Text(FText::FromString(TEXT("Run Export")))
								.OnClicked(this, &SUMGAutoBuilderWorkbench::OnExportClicked)
							]
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 12.0f, 0.0f, 12.0f)
						[
							SNew(SSeparator)
						]

						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Result")))
						]

						+ SVerticalBox::Slot()
						.FillHeight(1.0f)
						.Padding(0.0f, 6.0f, 0.0f, 0.0f)
						[
							SAssignNew(OutputTextBox, SMultiLineEditableTextBox)
							.IsReadOnly(true)
							.AlwaysShowScrollbars(true)
						]
					]
				]
			];

			SetOutput(TEXT("Ready.\nUse Build/Patch for JSON-driven updates, or Export to scaffold a new JSON spec."));
		}

	private:
		FReply OnBuildClicked()
		{
			RunBuild(TEXT("build"));
			return FReply::Handled();
		}

		FReply OnPatchClicked()
		{
			RunBuild(TEXT("patch"));
			return FReply::Handled();
		}

		FReply OnExportClicked()
		{
			if (!WidgetPathTextBox.IsValid())
			{
				SetOutput(TEXT("Widget Path textbox is not available."));
				return FReply::Handled();
			}

			const FString WidgetPath = WidgetPathTextBox->GetText().ToString().TrimStartAndEnd();
			if (WidgetPath.IsEmpty())
			{
				SetOutput(TEXT("Please provide a Widget Path before running Export."));
				return FReply::Handled();
			}

			const FString ExportPath = ExportPathTextBox.IsValid() ? ExportPathTextBox->GetText().ToString().TrimStartAndEnd() : FString();
			FUMGAutoBuilderReport Report;
			const bool bOk = FUMGAutoBuilderService::ExportWidgetToJsonFile(WidgetPath, ExportPath, bPrettyExport, Report);
			SetOutput(FormatReport(Report, bOk));
			return FReply::Handled();
		}

		void RunBuild(const FString& Mode)
		{
			if (!ConfigPathTextBox.IsValid())
			{
				SetOutput(TEXT("Config Path textbox is not available."));
				return;
			}

			const FString ConfigPath = ConfigPathTextBox->GetText().ToString().TrimStartAndEnd();
			if (ConfigPath.IsEmpty())
			{
				SetOutput(TEXT("Please provide a Config Path before running Build/Patch."));
				return;
			}

			FUMGAutoBuilderReport Report;
			const bool bOk = FUMGAutoBuilderService::BuildFromConfigFile(ConfigPath, Mode, Report);
			SetOutput(FormatReport(Report, bOk));
		}

		void SetOutput(const FString& Text) const
		{
			if (OutputTextBox.IsValid())
			{
				OutputTextBox->SetText(FText::FromString(Text));
			}
		}

		void OnPrettyExportChanged(const ECheckBoxState NewState)
		{
			bPrettyExport = (NewState == ECheckBoxState::Checked);
		}

		ECheckBoxState GetPrettyExportCheckState() const
		{
			return bPrettyExport ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
		}

	private:
		TSharedPtr<SEditableTextBox> ConfigPathTextBox;
		TSharedPtr<SEditableTextBox> WidgetPathTextBox;
		TSharedPtr<SEditableTextBox> ExportPathTextBox;
		TSharedPtr<SMultiLineEditableTextBox> OutputTextBox;
		bool bPrettyExport = true;
	};
}

IMPLEMENT_MODULE(FUMGAutoBuilderEditorModule, UMGAutoBuilderEditor)

void FUMGAutoBuilderEditorModule::StartupModule()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		UMGAutoBuilderTabName,
		FOnSpawnTab::CreateRaw(this, &FUMGAutoBuilderEditorModule::SpawnBuilderTab))
		.SetDisplayName(FText::FromString(TEXT("UMG Auto Builder")))
		.SetTooltipText(FText::FromString(TEXT("Open the UMG Auto Builder workbench.")));

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FUMGAutoBuilderEditorModule::RegisterMenus));
}

void FUMGAutoBuilderEditorModule::ShutdownModule()
{
	if (UToolMenus::TryGet())
	{
		UToolMenus::UnRegisterStartupCallback(this);
		UToolMenus::UnregisterOwner(this);
	}

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(UMGAutoBuilderTabName);
}

void FUMGAutoBuilderEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.MainMenu.Tools"));
	FToolMenuSection& Section = Menu->FindOrAddSection(TEXT("UMGAutoBuilder"));
	Section.AddMenuEntry(
		TEXT("OpenUMGAutoBuilder"),
		FText::FromString(TEXT("UMG Auto Builder")),
		FText::FromString(TEXT("Open the UMG Auto Builder workbench.")),
		FSlateIcon(),
		FToolMenuExecuteAction::CreateLambda([](const FToolMenuContext&)
		{
			FGlobalTabmanager::Get()->TryInvokeTab(UMGAutoBuilderTabName);
		}));
}

TSharedRef<SDockTab> FUMGAutoBuilderEditorModule::SpawnBuilderTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SUMGAutoBuilderWorkbench)
		];
}


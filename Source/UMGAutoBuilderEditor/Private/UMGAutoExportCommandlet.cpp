#include "UMGAutoExportCommandlet.h"

#include "Misc/Parse.h"
#include "UMGAutoBuilderService.h"

UUMGAutoExportCommandlet::UUMGAutoExportCommandlet()
{
	IsClient = false;
	IsEditor = true;
	LogToConsole = true;
}

int32 UUMGAutoExportCommandlet::Main(const FString& Params)
{
	FString WidgetPath;
	if (!FParse::Value(*Params, TEXT("Widget="), WidgetPath))
	{
		UE_LOG(LogTemp, Error, TEXT("[UMGAutoExport] Missing param: -Widget=<WidgetBlueprintPath>"));
		return 1;
	}
	WidgetPath.TrimQuotesInline();

	FString OutPath;
	FParse::Value(*Params, TEXT("Out="), OutPath);
	OutPath.TrimQuotesInline();

	const bool bPretty = FParse::Param(*Params, TEXT("Pretty"));

	FUMGAutoBuilderReport Report;
	const bool bOk = FUMGAutoBuilderService::ExportWidgetToJsonFile(WidgetPath, OutPath, bPretty, Report);

	for (const FString& Warning : Report.Warnings)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMGAutoExport] %s"), *Warning);
	}

	if (!bOk)
	{
		for (const FString& Error : Report.Errors)
		{
			UE_LOG(LogTemp, Error, TEXT("[UMGAutoExport] %s"), *Error);
		}
		return 2;
	}

	UE_LOG(LogTemp, Display, TEXT("[UMGAutoExport] %s"), *Report.Summary);
	return 0;
}


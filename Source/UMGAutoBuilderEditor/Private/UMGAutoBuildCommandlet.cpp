#include "UMGAutoBuildCommandlet.h"

#include "Misc/Parse.h"
#include "UMGAutoBuilderService.h"

UUMGAutoBuildCommandlet::UUMGAutoBuildCommandlet()
{
	IsClient = false;
	IsEditor = true;
	LogToConsole = true;
}

int32 UUMGAutoBuildCommandlet::Main(const FString& Params)
{
	FString ConfigArg;
	if (!FParse::Value(*Params, TEXT("Config="), ConfigArg))
	{
		UE_LOG(LogTemp, Error, TEXT("[UMGAutoBuilder] 缺少参数：-Config=<json path>"));
		return 1;
	}

	FString Mode = TEXT("build");
	FParse::Value(*Params, TEXT("Mode="), Mode);
	Mode.TrimQuotesInline();

	FUMGAutoBuilderReport Report;
	const bool bOk = FUMGAutoBuilderService::BuildFromConfigFile(ConfigArg, Mode, Report);

	for (const FString& Warning : Report.Warnings)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMGAutoBuilder] %s"), *Warning);
	}

	if (!bOk)
	{
		for (const FString& Error : Report.Errors)
		{
			UE_LOG(LogTemp, Error, TEXT("[UMGAutoBuilder] %s"), *Error);
		}
		return 5;
	}

	UE_LOG(LogTemp, Display, TEXT("[UMGAutoBuilder] %s"), *Report.Summary);
	return 0;
}


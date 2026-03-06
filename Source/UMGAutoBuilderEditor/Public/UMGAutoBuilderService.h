#pragma once

#include "CoreMinimal.h"

struct FUMGAutoBuilderReport
{
	TArray<FString> Warnings;
	TArray<FString> Errors;
	FString Summary;

	bool HasErrors() const
	{
		return Errors.Num() > 0;
	}
};

class FUMGAutoBuilderService
{
public:
	enum
	{
		CurrentSchemaVersion = 1
	};

	static bool BuildFromConfigFile(const FString& ConfigPath, const FString& OverrideMode, FUMGAutoBuilderReport& OutReport);
	static bool ExportWidgetToJsonFile(const FString& WidgetPath, const FString& OutPath, bool bPretty, FUMGAutoBuilderReport& OutReport);
};


#pragma once

#include "Commandlets/Commandlet.h"
#include "UMGAutoBuildCommandlet.generated.h"

/**
 * Config-driven WidgetBlueprint builder.
 *
 * Usage (example):
 *   UnrealEditor-Cmd.exe "<Project>.uproject" -run=UMGAutoBuild -Config="<abs or rel json path>" -NoAssetRegistryCache -unattended -nop4
 *
 * Optional:
 *   -Mode=build|patch
 */
UCLASS()
class UUMGAutoBuildCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	UUMGAutoBuildCommandlet();

	virtual int32 Main(const FString& Params) override;
};


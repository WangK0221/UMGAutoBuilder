#pragma once

#include "Commandlets/Commandlet.h"
#include "UMGAutoExportCommandlet.generated.h"

/**
 * Export an existing WidgetBlueprint's WidgetTree to a JSON spec.
 *
 * Usage:
 *   UnrealEditor-Cmd.exe "<Project>.uproject" -run=UMGAutoExport -Widget="/Game/UI/WB_Example.WB_Example" -Out="C:/tmp/WB_Example.json" -Pretty
 */
UCLASS()
class UUMGAutoExportCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	UUMGAutoExportCommandlet();

	virtual int32 Main(const FString& Params) override;
};


#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FUMGAutoBuilderEditorModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterMenus();
	TSharedRef<class SDockTab> SpawnBuilderTab(const class FSpawnTabArgs& Args);
};


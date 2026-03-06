using UnrealBuildTool;

public class UMGAutoBuilderEditor : ModuleRules
{
    public UMGAutoBuilderEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "UMG",
                "UMGEditor",
                "Slate",
                "SlateCore",
                "Json",
                "JsonUtilities",
                "UnrealEd",
                "BlueprintGraph",
                "Kismet",
                "KismetCompiler",
                "AssetTools",
                "ToolMenus",
                "Projects",
            }
        );
    }
}


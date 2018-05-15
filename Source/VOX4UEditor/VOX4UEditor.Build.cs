// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

using UnrealBuildTool;

public class VOX4UEditor : ModuleRules
{
    public VOX4UEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "VOX4UEditor/Public"
            }
        );


        PrivateIncludePaths.AddRange(
            new string[] {
                "VOX4UEditor/Private",
            }
        );


        PublicDependencyModuleNames.AddRange(
            new string[] {
                "UnrealEd",
                "CoreUObject",
                "Engine",
                "Core",
                "InputCore",
                "Slate",
                "SlateCore",
                "UnrealEd",
                "RawMesh",
                "MainFrame",
                "PropertyEditor",
                "AssetTools",
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "VOX4U",
                "ApexDestruction",
            }
        );


        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
            }
        );
    }
}

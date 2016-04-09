// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VOX4UEditor : ModuleRules
{
    public VOX4UEditor(TargetInfo Target) {

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
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "VOX4U",
            }
        );


        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
            }
        );
    }
}

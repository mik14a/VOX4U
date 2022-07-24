// Copyright 2016-2018 mik14a / Admix Network. All Rights Reserved.

using UnrealBuildTool;

public class VOX4UEditor : ModuleRules
{
	public VOX4UEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.V2;
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"VOX4U",
				"CoreUObject",
				"Engine",
				"RawMesh",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"MainFrame",
			}
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}

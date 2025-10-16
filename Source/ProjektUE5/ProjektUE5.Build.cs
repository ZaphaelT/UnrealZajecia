// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjektUE5 : ModuleRules
{
	public ProjektUE5(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ProjektUE5",
			"ProjektUE5/Variant_Platforming",
			"ProjektUE5/Variant_Platforming/Animation",
			"ProjektUE5/Variant_Combat",
			"ProjektUE5/Variant_Combat/AI",
			"ProjektUE5/Variant_Combat/Animation",
			"ProjektUE5/Variant_Combat/Gameplay",
			"ProjektUE5/Variant_Combat/Interfaces",
			"ProjektUE5/Variant_Combat/UI",
			"ProjektUE5/Variant_SideScrolling",
			"ProjektUE5/Variant_SideScrolling/AI",
			"ProjektUE5/Variant_SideScrolling/Gameplay",
			"ProjektUE5/Variant_SideScrolling/Interfaces",
			"ProjektUE5/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

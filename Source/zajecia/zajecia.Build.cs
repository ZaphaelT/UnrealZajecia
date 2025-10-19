// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class zajecia : ModuleRules
{
	public zajecia(ReadOnlyTargetRules Target) : base(Target)
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
			"zajecia",
			"zajecia/Variant_Platforming",
			"zajecia/Variant_Platforming/Animation",
			"zajecia/Variant_Combat",
			"zajecia/Variant_Combat/AI",
			"zajecia/Variant_Combat/Animation",
			"zajecia/Variant_Combat/Gameplay",
			"zajecia/Variant_Combat/Interfaces",
			"zajecia/Variant_Combat/UI",
			"zajecia/Variant_SideScrolling",
			"zajecia/Variant_SideScrolling/AI",
			"zajecia/Variant_SideScrolling/Gameplay",
			"zajecia/Variant_SideScrolling/Interfaces",
			"zajecia/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

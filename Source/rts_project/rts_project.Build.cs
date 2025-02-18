// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class rts_project : ModuleRules
{
	public rts_project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "UMG", "Engine", "InputCore", "EnhancedInput",
			"Slate", "SlateCore", "AIModule", "NavigationSystem", "GameplayTasks"});
	}
}

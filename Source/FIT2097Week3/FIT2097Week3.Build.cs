// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FIT2097Week3 : ModuleRules
{
	public FIT2097Week3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "NavigationSystem", "GameplayTasks" });
	}
}

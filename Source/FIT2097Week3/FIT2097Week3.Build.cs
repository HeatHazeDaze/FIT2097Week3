// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FIT2097Week3 : ModuleRules
{
	public FIT2097Week3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "NavigationSystem", "GameplayTasks", "Niagara" });

        PublicIncludePaths.AddRange(new string[] {"FIT2097Week3/",
            "C:/Program Files/Epic Games/UE_4.25/Engine/Plugins/FX/Niagara/Source/Niagara/Public",
            "C:/Program Files/Epic Games/UE_4.25/Engine/Plugins/FX/Niagara/Source/Niagara/Classes",
            "C:/Program Files/Epic Games/UE_4.25/Engine/Plugins/FX/Niagara/Source/Niagara/Private"});
	}
}

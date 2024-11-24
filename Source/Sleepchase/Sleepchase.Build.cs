// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Sleepchase : ModuleRules
{
	public Sleepchase(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"AIModule", 
			"GameplayTasks", 
			"UMG", 
			"GameplayTags", 
			"OnlineSubsystem", 
			"DeveloperSettings",
			"SignificanceManager",
			"EnhancedInput",
			"Niagara",
			"CoreOnline"
		});
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Doors : ModuleRules
{
	public Doors(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(["Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput"]);
		
		PrivateDependencyModuleNames.AddRange(["AIModule", "NavigationSystem"]);
	}
}

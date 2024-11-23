// Copyright Philip Dunstan 2023

using UnrealBuildTool;

public class DoorsEditor : ModuleRules
{
	public DoorsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicIncludePaths.AddRange(new [] { "DoorsEditor" });

		PublicDependencyModuleNames.AddRange(new [] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		PublicDependencyModuleNames.AddRange(new [] { "Doors" });
	}
}

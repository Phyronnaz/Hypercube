// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Hypercube : ModuleRules
{
	public Hypercube(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "CustomMeshComponent" });
        PrivateIncludePathModuleNames.AddRange(new string[] { "CustomMeshComponent" });
    }
}

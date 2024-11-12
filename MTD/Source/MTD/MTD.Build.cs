// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MTD : ModuleRules
{
    public MTD(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        { 
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "AIModule",
            "NavigationSystem",
            "UMG"
        });
    }
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PlayWidgetAnimation : ModuleRules
{
	public PlayWidgetAnimation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		IWYUSupport = IWYUSupport.Full;
		// bUseUnity = false;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine", 
				"UMG", 
			}
			);
	}
}

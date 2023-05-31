// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PlayWidgetAnimationEditor : ModuleRules
{
	public PlayWidgetAnimationEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		IWYUSupport = IWYUSupport.Full;
		// bUseUnity = false;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"PlayWidgetAnimation",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"BlueprintGraph",
			}
			);
	}
}

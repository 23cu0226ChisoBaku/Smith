// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Smith : ModuleRules
{
	public Smith(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", 
																												"InputCore", "EnhancedInput", "MLibrary", 
																												"TurnBattleSystem" , "MapManagement", "SmithModel",
																												"Niagara", "SmithModelInitializer", "SmithGod"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PrivateIncludePaths.AddRange(new string[] 
																						{
																							"Smith/BattleCommand",
																							"Smith/Player",
																							"Smith/AI",
																							"Smith/UI",
																							"Smith/Components",
																							"Smith/Event",
																							"Smith/Enemies",
																							"Smith/SmithActors",
																							"Smith",
																							"Smith/Pickable",
																						});

		CppStandard = CppStandardVersion.Latest;
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });
		
	}
}

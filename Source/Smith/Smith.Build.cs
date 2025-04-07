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
																												"Niagara", "SmithModelInitializer", "SmithGameplay"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PrivateIncludePaths.AddRange(new string[] 
																						{
																							"Smith/AI",
																							"Smith/BattleCommand",
																							"Smith/Components",
																							"Smith/Enemies",
																							"Smith/Equipment",
																							"Smith/Event",
																							"Smith/Models",
																							"Smith/Pickable",
																							"Smith/Player",
																							"Smith/UI",
																							"SmithModel/DataTableTypes",
																							"SmithModel/Models",
																						});

		CppStandard = CppStandardVersion.Latest;
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });
		
	}
}

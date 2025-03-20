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
																												"SmithGod", "Niagara", "SmithModelInitializer"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PrivateIncludePaths.AddRange(new string[] 
																						{
																							"Smith/BattleCommand",
																							"Smith/Player",
																							"Smith/SmithActors/Weapon",
																							"Smith/SmithActors/TurnMngObj/Public",
																							"Smith/SmithActors/Interfaces",
																							"Smith/SmithActors",
																							"Smith",
																						});

		CppStandard = CppStandardVersion.Latest;
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });
		
	}
}

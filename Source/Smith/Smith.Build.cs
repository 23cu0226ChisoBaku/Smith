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
																												"SmithGod", "Niagara"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PrivateIncludePaths.AddRange(new string[] 
																						{
																							"Smith/BattleCommand/Public",
																							"Smith/BattleCommand",
																							"Smith/SmithActors/Player/Public",
																							"Smith/SmithActors/Weapon",
																							"Smith/SmithActors/TurnMngObj/Public",
																							"Smith/SmithActors/Interfaces",
																							"Smith/SmithActors",
																							"Smith/SmithActors/Weapon",
																							"Smith",
																						});

		CppStandard = CppStandardVersion.Latest;
		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

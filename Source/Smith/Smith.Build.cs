// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Smith : ModuleRules
{
	public Smith(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "MLibrary" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PrivateIncludePaths.AddRange(new string[] 
																						{ "Smith/TurnSystem/Public",
																							"Smith/TurnSystem/Classes",
																							"Smith/TurnSystem",
																							"Smith/BattleCommand/Public",
																							"Smith/BattleCommand",
																							"Smith/SmithActors/Player/Public",
																							"Smith/SmithActors/TurnMngObj/Public",
                                                                                            "Smith/SmithActors/Weapon/Public",
                                                                                            "Smith/SmithActors",
																							"Smith",
																							"Smith/Models/Public",	  
																						});

		CppStandard = CppStandardVersion.Latest;
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

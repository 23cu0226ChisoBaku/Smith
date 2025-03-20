// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SmithGod: ModuleRules
{
  public SmithGod(ReadOnlyTargetRules Target) : base(Target)
  {

    PublicDependencyModuleNames.AddRange(new string[] {"Core" , "CoreUObject" , "Engine", 
                                                       "MLibrary", "TurnBattleSystem", "MapManagement", 
                                                       "SmithModel", "Smith", "UMG" , "Niagara", "SmithModelInitializer"});

    PrivateIncludePaths.AddRange(new string[] {"Smith/SmithActors/Interfaces",
                                               "Smith/BattleCommand",
                                               "Smith/SmithActors/TurnMngObj/Public",
                                               "Smith/Player"});
  }

}
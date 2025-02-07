// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SmithGod: ModuleRules
{
  public SmithGod(ReadOnlyTargetRules Target) : base(Target)
  {

    PublicDependencyModuleNames.AddRange(new string[] {"Core" , "CoreUObject" , "Engine", 
                                                       "MLibrary", "TurnBattleSystem", "MapManagement", 
                                                       "SmithModel", "Smith", "UMG" , "Niagara"});

    PrivateIncludePaths.AddRange(new string[] {"Smith/SmithActors/Interfaces",
                                               "Smith/BattleCommand/Public",
                                               "Smith/SmithActors/TurnMngObj/Public",
                                               "Smith/SmithActors/Player/Public"});
  }

}
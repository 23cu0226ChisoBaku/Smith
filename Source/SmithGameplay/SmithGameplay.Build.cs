// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SmithGameplay: ModuleRules
{
  public SmithGameplay(ReadOnlyTargetRules Target) : base(Target)
  {

    PublicDependencyModuleNames.AddRange(new string[] {"Core" , "CoreUObject" , "Engine", 
                                                       "MLibrary", "TurnBattleSystem", "MapManagement", 
                                                       "SmithModel", "Smith", "UMG" , "Niagara", "SmithModelInitializer"});

    PrivateIncludePaths.AddRange(
                                  new string[]{ 
                                                "Smith",
                                                "Smith/BattleCommand",
                                                "Smith/UI",
                                                "Smith/AI",
                                                "Smith/Enemies",
                                                "Smith/Player",
                                                "Smith/Event",
                                                "SmithModel/DataTableTypes",
                                                "SmithModel/Models",
                                              }
                                );
  }

}
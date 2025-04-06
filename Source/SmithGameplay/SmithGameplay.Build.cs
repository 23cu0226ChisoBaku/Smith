// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SmithGameplay: ModuleRules
{
  public SmithGameplay(ReadOnlyTargetRules Target) : base(Target)
  {

    PublicDependencyModuleNames.AddRange(new string[] {"Core" , "CoreUObject" , "Engine", 
                                                       "MLibrary", "TurnBattleSystem", "MapManagement", 
                                                       "SmithModel", "Smith", "UMG" , "Niagara", "SmithModelInitializer", "SmithDatabase"});

    PublicIncludePaths.AddRange(
                                  new string[]{ 
                                                "Smith",
                                                "Smith/BattleCommand",
                                                "Smith/UI",
                                                "Smith/AI",
                                                "Smith/Enemies",
                                                "Smith/Player",
                                                "Smith/Event",
                                                "Smith/Models",
                                                "SmithModel/DataTableTypes",
                                                "SmithModel/Models",
                                              }
                                );
  }

}
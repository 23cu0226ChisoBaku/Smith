// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TurnBattleSystem: ModuleRules
{
  public TurnBattleSystem(ReadOnlyTargetRules Target) : base(Target)
  {

    PublicDependencyModuleNames.AddRange(new string[] {"Core" , "CoreUObject" , "Engine", "MLibrary" ,"SmithModel", "Smith", "MapManagement", "SmithGameplay"});

    PrivateIncludePaths.AddRange(new string[] {"Smith/BattleCommand"});
  }

}
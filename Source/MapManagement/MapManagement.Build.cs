// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MapManagement: ModuleRules
{
  public MapManagement(ReadOnlyTargetRules Target) : base(Target)
  {

    PublicDependencyModuleNames.AddRange(new string[] {"Core" , "CoreUObject" , "Engine", "MLibrary", "SmithModel", "Smith"});

    PrivateIncludePaths.AddRange(new string[] 
                                            {
                                              "Smith/BattleCommand",
                                              "Smith/SmithActors/TurnMngObj/Public",
                                              "Smith/SmithActors/Interfaces",
                                              "Smith/SmithActors",
                                              "Smith",
                                              "MapManagement/MapCore",
                                              "MapManagement/MapDataModel",
                                              "MapManagement/MapFacade",
                                              "MapManagement/Utils",
                                            });
  }

}
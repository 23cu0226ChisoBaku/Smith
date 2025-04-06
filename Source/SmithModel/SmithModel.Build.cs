// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SmithModel: ModuleRules
{
  public SmithModel(ReadOnlyTargetRules Target) : base(Target)
  {

    PublicDependencyModuleNames.AddRange(new string[] { "Core" , "CoreUObject", 
                                                        "Engine", "MLibrary", 
                                                        "SmithGameplay", "Smith", 
                                                        "MapManagement", });

    PrivateIncludePaths.AddRange(
                                  new string[]
                                  {
                                    "SmithModel/DataTableTypes",
                                    "SmithModel/Models",
                                  }
                                );

    PublicIncludePaths.AddRange(
                                  new string[]
                                  {
                                    "MapManagement/MapDataModel",
                                    "Smith/Models",
                                  }
                               );

  }

}
// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SmithModel: ModuleRules
{
  public SmithModel(ReadOnlyTargetRules Target) : base(Target)
  {

    PublicDependencyModuleNames.AddRange(new string[] {"Core" , "CoreUObject" , "Engine", "MLibrary", "SmithGod", "Smith" });

    PrivateIncludePaths.AddRange(
                                  new string[]
                                  {
                                    "SmithModel/DataTableTypes",
                                    "SmithModel/Models",
                                  }
                                );

  }

}
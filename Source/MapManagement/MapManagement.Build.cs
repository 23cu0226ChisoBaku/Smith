// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MapManagement: ModuleRules
{
  public MapManagement(ReadOnlyTargetRules Target) : base(Target)
  {

    PublicDependencyModuleNames.AddRange(new string[] {"Core" , "CoreUObject" , "Engine", "MLibrary", "SmithModel", "SmithGod"});

  }

}
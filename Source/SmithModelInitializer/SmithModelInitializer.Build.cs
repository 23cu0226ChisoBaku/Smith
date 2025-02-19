// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SmithModelInitializer: ModuleRules
{
  public SmithModelInitializer(ReadOnlyTargetRules Target) : base(Target)
  {

    PublicDependencyModuleNames.AddRange(new string[] {"Core" , "CoreUObject" , "Engine", "MLibrary", "Smith"});

  }

}
// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SmithDatabase : ModuleRules
{
	public SmithDatabase(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(
                                          new string[] 
                                          { "Core", 
                                            "CoreUObject", 
                                            "Engine", 
                                            "SmithModel",
                                            "SmithGameplay",
                                            "MapManagement",
                                          }
                                        );

    PrivateIncludePaths.AddRange(
                                  new string[]
                                  {
                                    "MapManagement/MapDataModel",
                                    "SmithGameplay/Models",
                                  }
                                );

		CppStandard = CppStandardVersion.Latest;
		
	}
}

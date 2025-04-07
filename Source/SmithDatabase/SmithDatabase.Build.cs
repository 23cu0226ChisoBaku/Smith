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
                                            "Smith",
                                            "SmithModel",
                                            "SmithGameplay",
                                            "MapManagement",
                                          }
                                        );

    PublicIncludePaths.AddRange(
                                  new string[]
                                  {
                                    "MapManagement/MapDataModel",
                                    "SmithGameplay/Models",
                                    "Smith/Equipment",
                                  }
                                );

    PrivateDependencyModuleNames.AddRange(
                                            new string[]
                                            {
                                              "Smith",
                                            }
                                         );
    
    PrivateIncludePaths.AddRange(
                                  new string[]
                                  {
                                    
                                  }
                                );

		CppStandard = CppStandardVersion.Latest;
		
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerator.h"
#include "Debug.h"

// UMapGenerator::UMapGenerator()
//   : m_mapBuffer({})
//   { }

// bool UMapGenerator::GenerateMap(UWorld* world, const FString& mapMetaDataPath, FMapCoord originCoord)
// {
//   if (!::IsValid(world))
//   {
//     MDebug::LogError("Invalid World ------ Generate Map");
//     return false;
//   }

//   if (mapMetaDataPath.IsEmpty())
//   {
//     MDebug::LogError("Empty Map Path ------ Generate Map");
//     return false;
//   }

//   uint32 pathHash = GetTypeHash(mapMetaDataPath);

//   if (!m_mapBuffer.Contains(pathHash))
//   {
//     TSoftObjectPtr<UDataTable> m_map = LoadObject<UDataTable>(world, *mapMetaDataPath); 

//     if (!m_map.IsValid())
//     {
//       m_map.LoadSynchronous();
//     }

//     UDataTable* dataTable = m_map.Get();

//     if (!::IsValid(dataTable))
//     {
//       MDebug::LogError("Can not load data ------ Generate Map");
//       MDebug::LogError(TEXT("File Path : ") + mapMetaDataPath);
//       return false;
//     }

//     return true;
//   }

//   return false;
// }

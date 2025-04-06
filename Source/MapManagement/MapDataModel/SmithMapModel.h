// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_MAP_MODEL
#define SMITH_MAP_MODEL

#include "MapModelType.h"

class AActor;

/// @brief ダンジョンモデル
struct MAPMANAGEMENT_API FSmithMapModel
{
  friend class USmithMapModelRepository;

  private:
  
    FSmithMapModel();

    static FSmithMapModel CreateModel(UClass* ActorClass, uint8 SizeX, uint8 SizeY, EMapModelType MapModelType);

  public:

    uint8 GetSizeX() const
    {
      return m_sizeX;
    }

    uint8 GetSizeY() const
    {
      return m_sizeY;
    }

    EMapModelType GetModelType() const
    {
      return m_type;
    }

    bool IsValid() const;

  private:

    TSubclassOf<AActor> m_actorClass; 
    uint8 m_sizeX;
    uint8 m_sizeY;
    EMapModelType m_type;

};

#endif
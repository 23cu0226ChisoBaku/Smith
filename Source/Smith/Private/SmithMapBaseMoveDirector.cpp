// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapBaseMoveDirector.h"

#include "ITargetTracker.h"
#include "MLibrary.h"

USmithMapBaseMoveDirector::USmithMapBaseMoveDirector(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_targetTracker(nullptr)
  , m_chaser(nullptr)
  , m_chaseRadius(0)
{ }

void USmithMapBaseMoveDirector::Initialize(ITargetTracker* targetChaser, AActor* chaser, uint8 chaseRadius)
{
  m_targetTracker = targetChaser;
  m_chaser = chaser;
  m_chaseRadius = chaseRadius;
}

EDirection USmithMapBaseMoveDirector::getNextDirectionImpl()
{
  EDirection trackResult = EDirection::Invalid;

  if (!m_targetTracker.IsValid())
  {
    MDebug::LogError("Target Chaser is NULL");
    return trackResult;
  }

  if (!m_chaser.IsValid())
  {
    MDebug::LogError("Chaser is NULL");
    return trackResult;
  }

  m_targetTracker->TrackTarget(trackResult, m_chaser.Get(), m_chaseRadius);

  return trackResult;
}
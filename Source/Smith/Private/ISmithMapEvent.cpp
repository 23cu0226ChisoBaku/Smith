// Fill out your copyright notice in the Description page of Project Settings.


#include "ISmithMapEvent.h"

// Add default functionality here for any ISmithMapEvent functions that are not pure virtual.

void ISmithMapEvent::SetEventCoord(uint8 x, uint8 y)
{
  m_eventCoordX = x;
  m_eventCoordY = y;
}

uint8 ISmithMapEvent::GetEventCoordX() const
{
  return m_eventCoordX;
} 

uint8 ISmithMapEvent::GetEventCoordY() const
{
  return m_eventCoordY;
}
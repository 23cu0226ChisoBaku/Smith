// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithActors/Weapon/TestWeapon.h"
#include "Debug.h"

TestWeapon::TestWeapon()
:m_params{10,5,0,0}
{
  MDebug::Log("HP " + FString::FromInt(m_params.HP));
  MDebug::Log("ATK " + FString::FromInt(m_params.ATK));
  MDebug::Log("DEF " + FString::FromInt(m_params.DEF));
  MDebug::Log("CRT " + FString::FromInt(m_params.CRT));
}

TestWeapon::~TestWeapon()
{
}

FParams TestWeapon::GetParam()
{
  return m_params;
}

void TestWeapon::SetParam(FParams param)
{
  m_params = param;

  MDebug::LogError("Enhance Resulut");
  MDebug::LogError("HP " + FString::FromInt(m_params.HP));
  MDebug::LogError("ATK " + FString::FromInt(m_params.ATK));
  MDebug::LogError("DEF " + FString::FromInt(m_params.DEF));
  MDebug::LogError("CRT " + FString::FromInt(m_params.CRT));
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEnhance.h"
#include "Debug.h"

TestEnhance::TestEnhance()
:m_wepon(0)
,m_item(0)
{
}

TestEnhance::~TestEnhance()
{
}

void TestEnhance::Enhance()
{
  FString Str;
  Str = FString::FromInt(m_wepon + m_item);
  MDebug::LogError("Enhance Result" + Str);
}

void TestEnhance::SetEnhance(int32 Wepon,int32 Item)
{
m_wepon = Wepon;
m_item = Item;
}
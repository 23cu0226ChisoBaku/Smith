// Fill out your copyright notice in the Description page of Project Settings.


#include "TestItem.h"

TestItem::TestItem(FString name = "Name",int32 hp = 0,int32 atk = 0,int32 def = 0,int32 crt = 0)
:m_param{hp,atk,def,crt}
,m_name(name)
{
}

TestItem::~TestItem()
{
}

FParams TestItem::GetParam()
{
  return m_param;
}

FString TestItem::GetItemName()
{
  return m_name;
}

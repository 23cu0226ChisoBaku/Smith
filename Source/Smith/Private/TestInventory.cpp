// Fill out your copyright notice in the Description page of Project Settings.


#include "TestInventory.h"
#include "TestItem.h"

TestInventory::TestInventory()
{
  SetItem(TestItem(TEXT("牙"),10,0,10,0));
  SetItem(TestItem(TEXT("爪"),2,10,2,3));
  SetItem(TestItem(TEXT("翼"),1,1,1,1));
  SetItem(TestItem(TEXT("逆鱗"),10,20,10,30));
}

TestInventory::~TestInventory()
{
}

void TestInventory::SetItem(TestItem item)
{
  m_items.Emplace(item);
}

FParams TestInventory::GetItemParam(int32 select)
{
  if(select > m_items.Num())
  {
    return FParams {0,0,0,0};
  }
  return m_items[select].GetParam();
}

FString TestInventory::GetItemName(int32 select)
{
  if(select > m_items.Num())
  {
    return "Error Over";
  }
  return m_items[select].GetItemName();
}

int32 TestInventory::ItemNum()
{
  return m_items.Num();
}

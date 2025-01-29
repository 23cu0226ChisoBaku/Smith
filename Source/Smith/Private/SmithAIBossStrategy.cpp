// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithAIBossStrategy.h"
#include "SmithCommandFormat.h"
#include "FormatInfo_Import.h"

#include "MLibrary.h"

USmithAIBossStrategy::USmithAIBossStrategy(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_attackFormatTables{}
{ }

void USmithAIBossStrategy::BeginDestroy()
{
  Super::BeginDestroy();
  //全ての要素削除
  m_attackFormatTables.Empty();
}

// 攻撃のフォーマットの登録
bool USmithAIBossStrategy::RegisterAttackFormat(const FString& name, const UDataTable* formatTable)
{
  // すでに登録されているキーがあったら終わる
  if (m_attackFormatTables.Contains(name))
  {
    return false;
  }

  // フォーマットがなかったら終わり
  if (formatTable == nullptr)
  {
    return false;
  }

  TArray<FFormatInfo_Import*> arr;
  formatTable->GetAllRows<FFormatInfo_Import>("", arr);

  if (arr.Num() <= 1)
  {
    return false;
  }

  TArray<FName> names = formatTable->GetRowNames();
  if (!names[0].IsEqual(TEXT("FormatInfo")))
  {
    return false;
  }
  
  const uint8 formatRow = arr[0]->Row;
  const uint8 formatColumn = arr[0]->Column;
  const size_t dataCnt = formatRow * formatColumn;

  check((arr.Num() - 1) == StaticCast<int32>(dataCnt));
  if ((arr.Num() - 1) != StaticCast<int32>(dataCnt))
  {
    return false;
  }

  TArray<ESmithFormatType> typeSrcData;

  for (int i = 1; i < arr.Num(); ++i)
  {
    typeSrcData.Emplace(arr[i]->Type);
  }

  TSharedPtr<UE::Smith::Battle::FSmithCommandFormat> formatPtr = ::MakeShared<UE::Smith::Battle::FSmithCommandFormat>(typeSrcData.GetData(), dataCnt, formatRow, formatColumn);
  m_attackFormatTables.Emplace(name, formatPtr);
  
  return true;
}

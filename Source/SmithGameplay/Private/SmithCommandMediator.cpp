// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithCommandMediator.h"

#include "SmithBattleSubsystem.h"
#include "ITurnManageable.h"
#include "SmithDamageSubsystem.h"
#include "SmithCommandStrategies.h"
#include "SmithMapSystem.h"
#include "BattleCommandFactory.h"
#include "SmithCommandFormat.h"
#include "FormatTransformer.h"
#include "ISmithAnimator.h"
#include "InvalidValues.h"
#include "AttackHandle.h"
#include "SmithSkillParameter.h"
#include "FormatType.h"
#include "AttackableInfoHandle.h"
#include "SmithModelHelperFunctionLibrary.h"

#include "MLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithCommandMediator)

USmithCommandMediator::USmithCommandMediator()
  : m_battleSys(nullptr)
  , m_damageSys(nullptr)
  , m_mapSys(nullptr)
{ }

void USmithCommandMediator::BeginDestroy()
{
  Super::BeginDestroy();

  m_mapSys.Reset();
}

///
/// @brief              Mediatorを初期化
///
void USmithCommandMediator::SetupMediator(USmithBattleSubsystem* BattleSystem, USmithDamageSubsystem* DamageSystem, TSharedPtr<MapSystem> MapSys)
{
  check(BattleSystem != nullptr);
  check(DamageSystem != nullptr);
  check(MapSys != nullptr);
  
  m_battleSys = BattleSystem;
  m_damageSys = DamageSystem;

  m_mapSys.Reset();
  m_mapSys = MapSys;
  
}

///
/// @brief                移動コマンドを送る
///
bool USmithCommandMediator::SendMoveCommand(AActor* requester, EDirection moveDirection, uint8 moveDistance)
{
  check(requester != nullptr);
  if (m_battleSys == nullptr)
  {
    MDebug::LogError("Battle System INVALID!!!");
    return false;
  }

  TSharedPtr<MapSystem> mapSys_shared = m_mapSys.Pin();
  if (!mapSys_shared.IsValid())
  {
    MDebug::LogError("Map Manager INVALID!!!");
    return false;
  }
  
  FVector destination = FVector::ZeroVector;

  // マップ移動
  // 移動失敗の場合destinationに不正値が入る
  mapSys_shared->MoveMapObj(requester, moveDirection, moveDistance, destination);

  if (destination.Equals(InvalidValues::MAP_COORD_WORLD_INVALID))
  {
    return false;
  }

  // 移動コマンド作成
  {
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    SmithDefaultMoveStrategy moveStrategy = 
    {
      .MoveEntity = requester,
      .MoveSpeed = 800.0f,        // TODO
      .Destination = destination
    };
    m_battleSys->RegisterCommand(Cast<ITurnManageable>(requester), FBattleCommandFactory::CreateMoveCommand(moveStrategy, animator));
  }

  return true;
}

///
/// @brief                      攻撃コマンドを送る
///
bool USmithCommandMediator::SendAttackCommand(AActor* requester, EDirection direction, const UE::Smith::Battle::FSmithCommandFormat& format, const FAttackHandle& atkHandle, bool bAttackEvenNoTarget)
{
  check(requester != nullptr);
  check(format.IsValid());

  if ((m_battleSys == nullptr) || (m_damageSys == nullptr))
  {
    MDebug::LogError("System INVALID!!!");
    return false;
  }

  TSharedPtr<MapSystem> mapSys_shared = m_mapSys.Pin();
  if (!mapSys_shared.IsValid())
  {
    MDebug::LogError("Map Manager INVALID");
    return false;
  }

  using namespace UE::Smith::Battle;
  // 方向によってフォーマットを回転させる
  FSmithCommandFormat rotatedFormat = FFormatTransformer::GetRotatedFormat(format, direction);

  // 攻撃できる相手を探す
  // IAttackableを継承したオブジェクトは攻撃対象になる
  TArray<FAttackableInfoHandle> attackables{};
  mapSys_shared->FindAttackableMapObjs(attackables, requester, rotatedFormat);

  {
    ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);

    // 攻撃コマンドを登録
    if (attackables.Num() > 0)
    {
      for(const auto& attackableInfo : attackables)
      {
        SmithDefaultDamageStrategy damageStrategy{};
        damageStrategy.Instigator = requester;
        damageStrategy.Causer = attackableInfo.Attackable;
        damageStrategy.DamageApplicant = m_damageSys;
        damageStrategy.Handle = atkHandle;
        damageStrategy.FromDirection = attackableInfo.AttackFrom;
  
        m_battleSys->RegisterCommand(requesterTurnManageable, FBattleCommandFactory::CreateAttackCommand(damageStrategy, animator));
      }
      return true;
    }
  
    if (bAttackEvenNoTarget)
    {
      // Dummy攻撃コマンド
      SmithDummyStrategy dummy{};
      m_battleSys->RegisterCommand(requesterTurnManageable, FBattleCommandFactory::CreateAttackCommand(dummy, animator));
      return true;
    }

  }

  return false;
  
}

///
/// @brief                スキルコマンドを送る
///
bool USmithCommandMediator::SendSkillCommand(AActor* requester, FSmithSkillParameter skillParameter, const UE::Smith::Battle::FSmithCommandFormat& format, const FAttackHandle& atkHandle)
{
  check(requester != nullptr);
  check(format.IsValid());
  
  if ((m_battleSys == nullptr) || (m_damageSys == nullptr))
  {
    MDebug::LogError("System INVALID!!!");
    return false;
  }

  TSharedPtr<MapSystem> mapSys_shared = m_mapSys.Pin();
  if (!mapSys_shared.IsValid())
  {
    return false;
  }

  // TODO 攻撃コマンドと被る部分があるため、修正漏れの恐れがある
  // 設計し直す予定
  UE::Smith::Battle::FSmithCommandFormat rotatedFormat = UE::Smith::Battle::FFormatTransformer::GetRotatedFormat(format, skillParameter.ActiveDirection);
  TArray<FAttackableInfoHandle> attackables{};
  mapSys_shared->FindAttackableMapObjsFromCoord(/**Out */attackables, requester, rotatedFormat, skillParameter.OffsetToLeft, skillParameter.OffsetToTop);
  
  // スキルコマンドを送る
  {
    ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
    ISmithAnimator* animator = Cast<ISmithAnimator>(requester);
    if (attackables.Num() > 0)
    {
      for(const auto& attackableInfo : attackables)
      {
        SmithDefaultDamageStrategy damageStrategy{};
        damageStrategy.Instigator = requester;
        damageStrategy.Causer = attackableInfo.Attackable; 
        damageStrategy.DamageApplicant = m_damageSys;
        damageStrategy.Handle = atkHandle;
        damageStrategy.FromDirection = attackableInfo.AttackFrom;
  
        m_battleSys->RegisterCommand(requesterTurnManageable, FBattleCommandFactory::CreateSkillCommand(damageStrategy, animator, skillParameter.SkillSlot));
      }
      return true;
    }
    else
    {
      // Dummyコマンド
      SmithDummyStrategy dummy{};
      m_battleSys->RegisterCommand(requesterTurnManageable, FBattleCommandFactory::CreateSkillCommand(dummy, animator, skillParameter.SkillSlot));
    }
  }

  return false;
}

///
/// @brief  待機コマンドを送る
///
bool USmithCommandMediator::SendIdleCommand(AActor* requester, float idleTime)
{
  check(requester != nullptr);
  if (m_battleSys == nullptr)
  {
    return false;
  }

  ITurnManageable* requesterTurnManageable = Cast<ITurnManageable>(requester);
  if (requesterTurnManageable == nullptr)
  {
    return false;
  }

  m_battleSys->RegisterCommand(requesterTurnManageable, FBattleCommandFactory::CreateIdleCommand(idleTime));
  return true;
}

///
/// @brief  フォーマット影響範囲のワールド座標を取得
///
int32 USmithCommandMediator::GetRangeLocations(TArray<FVector>& outLocations, AActor* requester, FSmithSkillParameter skillParameter, const UE::Smith::Battle::FSmithCommandFormat& format)
{ 
  using namespace UE::Smith::Battle;
  check(requester != nullptr);
  check(format.IsValid());

  TSharedPtr<MapSystem> mapSys_shared = m_mapSys.Pin();
  if (!mapSys_shared.IsValid())
  {
    return 0;
  }
  
  outLocations.Reset();
  uint8 mapObjOriginCoordX = 0u;
  uint8 mapObjOriginCoordY = 0u;
  if (!mapSys_shared->GetMapObjectCoord(requester, mapObjOriginCoordX, mapObjOriginCoordY))
  {
    return 0;
  }

  // フォーマットを回転させる
  FSmithCommandFormat rotatedFormat = FFormatTransformer::GetRotatedFormat(format, skillParameter.ActiveDirection);
  auto formattedMapCoords = FFormatTransformer::FormatToMapCoord(rotatedFormat, FMapCoord(mapObjOriginCoordX + skillParameter.OffsetToLeft, mapObjOriginCoordY + skillParameter.OffsetToTop));

  // フォーマットから効果範囲を座標に変換する
  for (int32 row = 0; row < rotatedFormat.GetRow(); ++row)
  {
    for (int32 column = 0; column < rotatedFormat.GetColumn(); ++column)
    {
      using enum ESmithFormatType;
      switch(rotatedFormat.GetFormatData(column, row))
      {
        case EFFECT:
        case CENTER_EFFECT:
        {
          const FMapCoord coord = formattedMapCoords.At_ReadOnly(row, column);

          FVector location = FVector::ZeroVector;
          if (!mapSys_shared->ConvertMapCoordToWorldLocation(location, coord.x, coord.y))
          {
            continue;
          }

          outLocations.Emplace(location);
        }
        break;

        case NO_EFFECT:
        case CENTER_NO_EFFECT:
        default:
        {
          continue;
        }
      }
    }
  }

  return outLocations.Num();
}

///
/// @brief  プレイヤーが請求者のどの方向にいるかを取得する 
///
void USmithCommandMediator::GetPlayerDirection(EDirection& outDirection, EDirectionPolicy directionStrategy, AActor* requester, uint8 offsetToLeft, uint8 offsetToTop)
{
  check(requester != nullptr);

  TSharedPtr<MapSystem> mapSys_shared = m_mapSys.Pin();
  if (!mapSys_shared.IsValid())
  {
    return;
  }

  mapSys_shared->GetPlayerDirection(outDirection, directionStrategy, requester, offsetToLeft, offsetToTop);
}


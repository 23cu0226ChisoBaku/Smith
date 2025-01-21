// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithActors/TurnMngObj/Public/SmithEnemy.h"
#include "SmithMoveComponent.h"
#include "SmithAttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SmithPlayerActor.h"

#include "Debug.h"

ASmithEnemy::ASmithEnemy()
    : m_hp(5)
{
  SetTurnPriority(ETurnPriority::Rival);
  PrimaryActorTick.bCanEverTick = false;

  m_moveComp = CreateDefaultSubobject<USmithMoveComponent>(TEXT("konno Enemy Move Component"));
  check((m_moveComp != nullptr));

  //m_moveComp->SetMoveSpeed(250.0f);
  MOVE_DISTANCE = 100.0f;

  m_attackComp = CreateDefaultSubobject<USmithAttackComponent>(TEXT("Konno Enemy Attack Component"));
}

void ASmithEnemy::BeginPlay()
{
  Super::BeginPlay();

  // 指定したクラスのアクターを取得
  TArray<TObjectPtr<AActor>> aActorList;
  UGameplayStatics::GetAllActorsOfClass(this, ASmithPlayerActor::StaticClass(), aActorList);

  for (TObjectPtr<AActor> aActor : aActorList)
  {
    // 取得したクラスにキャスト
    m_target = Cast<ASmithPlayerActor>(aActor);
    break;
  }

  MDebug::LogWarning("Super Class BeginPlay");
}

void ASmithEnemy::OnAttack(AttackHandle &&handle)
{
  m_hp -= handle.AttackPower;
  MDebug::LogError(GetName() + TEXT(" left HP:") + FString::FromInt(m_hp));

  if (m_hp <= 0.0f)
  {
    Destroy();
  }
}

void ASmithEnemy::OnHeal(int32 heal)
{
  m_hp += heal;
  MDebug::LogError("Heal" + m_hp);
}

// プレイヤーが攻撃範囲内にプレイヤーがいるか確認
AActor *ASmithEnemy::PlayerCheck(float checkLenth)
{
  const float rayLenth = MOVE_DISTANCE;
    const float high = 100.0f;
  const FVector StartLocation = GetActorLocation() + FVector::UpVector * high;

  const FVector EndLocation[4] = {
      StartLocation + FVector::ForwardVector * rayLenth * checkLenth ,
      StartLocation + FVector::BackwardVector * rayLenth * checkLenth ,
      StartLocation + FVector::RightVector * rayLenth * checkLenth ,
      StartLocation + FVector::LeftVector * rayLenth * checkLenth ,
  };

  FHitResult HitResult;
  FCollisionQueryParams CollisionParams;
  CollisionParams.AddIgnoredActor(this);
  AActor *HitActor = nullptr;
  bool bHit = false;

  for (int i = 0; i < 4; ++i)
  {
    // レイキャスト
    bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation[i],
        ECC_MAX,
        CollisionParams);

    if (!bHit)
    {
      continue;
    }

    HitActor = HitResult.GetActor();

    // Playerにヒットしていたら攻撃
    if (::IsValid(HitActor) && Cast<IAttackable>(HitActor) != nullptr)
    {
      // デバッグ用レイ
      DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 1.0f);
      DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Blue, false, 1.0f, 0, 1.0f);

      MDebug::LogWarning(HitActor->GetName() + "Object");
      IAttackable *attackable = Cast<IAttackable>(HitActor);

      if (attackable != nullptr)
      {
        MDebug::Log("attackable");
        return HitActor;
      }
      else
      {
        MDebug::LogError("attack null " + GetName() + " HitObject " + HitActor->GetName());
        return nullptr;
      }
    }
  }

  // レイのデバッグ
  for (int i = 0; i < 4; ++i)
  {
    DrawDebugPoint(GetWorld(), EndLocation[i], 10.0f, FColor::Green, false, 1.0f);
    DrawDebugLine(GetWorld(), StartLocation, EndLocation[i], FColor::Green, false, 1.0f, 0, 1.0f);
  }

  return nullptr;
}

// 経路探索完成までの仮実装
uint8 ASmithEnemy::MoveDirection()
{
  FVector myPos = GetActorLocation();
  FVector targetPos;
  uint8 retPos = 0xff;
  if (m_target != nullptr)
  {
    targetPos = m_target->GetActorLocation();
  }

  if (targetPos.X > myPos.X)
  {
    retPos = 0;
  }
  else if (targetPos.X < myPos.X)
  {
    retPos = 4;
  }
  else if (targetPos.Y > myPos.Y)
  {
    retPos = 2;
  }
  else if (targetPos.Y < myPos.Y)
  {
    retPos = 6;
  }

  return retPos;
}
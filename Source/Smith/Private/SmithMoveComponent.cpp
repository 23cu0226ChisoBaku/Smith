
#include "SmithMoveComponent.h"
#include "MyPlayerCharacter.h"
#include "SmithPlayerActor.h"
#include "Kismet/GameplayStatics.h"

USmithMoveComponent::USmithMoveComponent()
{
}

void USmithMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	// 親Actorを取得
	m_enemyObj = GetOwner();
	// 指定したクラスのアクターを取得
	TArray<TObjectPtr<AActor>> aActorList;
	UGameplayStatics::GetAllActorsOfClass(this, ASmithPlayerActor::StaticClass(), aActorList);
	for (TObjectPtr<AActor> aActor : aActorList)
	{
		m_target = aActor;
		break;
	}
}

void USmithMoveComponent::Move()
{


	// ターゲット（プレイヤー）を取得
	if (m_target != nullptr)
	{
		m_targetPos = m_target->GetActorLocation();
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, m_target->GetName());
		}
	}
	else
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, TEXT("Target NULL"));
		}
		return;
	}

	// 自分の座標を取得
	m_myPos = m_enemyObj->GetActorLocation();

	// 移動
	if (m_targetPos.X > m_myPos.X)
	{
		m_myPos += FVector::ForwardVector * MOVE_DISTANCE;
	}
	else if (m_myPos.X > m_targetPos.X)
	{
		m_myPos += FVector::BackwardVector * MOVE_DISTANCE;
	}
	else if (m_targetPos.Y > m_myPos.Y)
	{
		m_myPos += FVector::RightVector * MOVE_DISTANCE;
	}
	else if (m_myPos.Y > m_targetPos.Y)
	{
		m_myPos += FVector::LeftVector * MOVE_DISTANCE;
	}

	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, TEXT("Enemy Move"));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, m_myPos.ToString());
	}
	// 座標の代入
	m_enemyObj->SetActorLocation(m_myPos);
}

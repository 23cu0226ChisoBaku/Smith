
#include "SmithMoveComponent.h"
#include "MyPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

USmithMoveComponent::USmithMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	m_enemyObj = GetOwner();
	m_temporaryTimer = 0.0f;
}

void USmithMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	// 自身のActorを取得
	if (m_enemyObj != nullptr)
	{
		m_myPos = m_enemyObj->GetActorLocation();
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, m_enemyObj->GetName());
		}
	}
	else
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, TEXT("Parent NULL"));
		}
	}

		// Playerを取得する仮の処理（コードが完成したら消す）
	TSubclassOf<AMyPlayerCharacter> findClass; // 見つけたいクラス
	findClass = AMyPlayerCharacter::StaticClass();
	TArray<AActor *> actorList;																				 // レベルに存在するすべてのアクターを格納する
	UGameplayStatics::GetAllActorsOfClass(this, findClass, actorList); // レベル内に存在するすべてのfindClassをactorListに入れる

	for (AActor *SomeActor : actorList)
	{
		// Playerを代入
		m_target = SomeActor;
				if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, m_target->GetName() + " Player Class");
		}
		break;
	}

}

void USmithMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	m_temporaryTimer += DeltaTime;
	if (m_temporaryTimer > 5.0f)
	{
		Move();
		m_temporaryTimer = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, "Enemy_MOVE");
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
	}

	
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

	// 座標の代入
	m_enemyObj->SetActorLocation(m_myPos);
}

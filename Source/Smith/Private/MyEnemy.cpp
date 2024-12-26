#include "MyEnemy.h"
#include "SmithMoveComponent.h"
#include "SmithAttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MoveCommand.h"
#include "AttackCommand.h"
#include "IAttackable.h"
#include "SmithPlayerActor.h"

#include "Debug.h"

// Sets default values
AMyEnemy::AMyEnemy()
	: m_hp(5.0f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetTurnPriority(ETurnPriority::Rival);

	m_moveComp = CreateDefaultSubobject<USmithMoveComponent>(TEXT("konno Enemy Move Component"));
	check((m_moveComp != nullptr));

	m_moveComp->SetMoveSpeed(250.0f);
	MOVE_DISTANCE = 250.0f;

	m_attackComp = CreateDefaultSubobject<USmithAttackComponent>(TEXT("Konno Enemy Attack Component"));
}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
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
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsCommandSendable())
	{
			m_timer += DeltaTime;
	}

	if (m_timer > 0.5f)
	{
		m_timer = 0.0f;
		PlayerCheck();
	}
}

void AMyEnemy::PlayerCheck()
{
	const float rayLenth = MOVE_DISTANCE;
	const FVector StartLocation = GetActorLocation();

	const FVector EndLocation[4] = {
		StartLocation + FVector::ForwardVector * rayLenth,
		StartLocation + FVector::BackwardVector * rayLenth,
		StartLocation + FVector::RightVector * rayLenth,
		StartLocation + FVector::LeftVector * rayLenth};

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
		if (::IsValid(HitActor))
		{
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 1.0f);
			DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Blue, false, 1.0f, 0, 1.0f);

			IAttackable* attackable = Cast<IAttackable>(HitActor);
			if (attackable != nullptr)
			{
				// TODO麦くんが直す
				return;
			}
		}
	}

	// レイのデバッグ
	for (int i = 0; i < 4; ++i)
	{
		DrawDebugPoint(GetWorld(), EndLocation[i], 10.0f, FColor::Green, false, 1.0f);
		DrawDebugLine(GetWorld(), StartLocation, EndLocation[i], FColor::Green, false, 1.0f, 0, 1.0f);
	}

	if (IsCommandSendable())
	{
		// 移動の処理
		m_moveComp->SetTerminusPos(MoveDirection());

	}
}

void AMyEnemy::OnAttack(AttackHandle&& handle)
{
	m_hp -= handle.AttackPower;

	MDebug::LogError(GetName() + TEXT(" left HP:") + FString::FromInt(m_hp));

	if (m_hp <= 0)
	{
		Destroy();
	}
}

FVector AMyEnemy::MoveDirection()
{
	FVector myPos = GetActorLocation();
	FVector targetPos;
	FVector retPos = FVector::ZeroVector;
	if (m_target != nullptr)
	{
		targetPos = m_target->GetActorLocation();
	}

	if (targetPos.X > myPos.X)
	{
		myPos += MOVE_DISTANCE * FVector::ForwardVector;
	}
	else if (targetPos.X < myPos.X)
	{
		myPos += MOVE_DISTANCE * FVector::BackwardVector;
	}
	else if (targetPos.Y > myPos.Y)
	{
		myPos += MOVE_DISTANCE * FVector::RightVector;
	}
	else if (targetPos.Y < myPos.Y)
	{
		myPos += MOVE_DISTANCE * FVector::LeftVector;
	}

	MDebug::Log(myPos.ToString());
	retPos = myPos;

	return retPos;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithActors/Player/SmithAnimPawnTest.h"
#include "Smith/Public/SmithAnimationComponent.h"
//#include "SmithGameOver/GameOver.h"

// Sets default values
ASmithAnimPawnTest::ASmithAnimPawnTest()

{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AnimationComp = CreateDefaultSubobject<USmithAnimationComponent>(TEXT("AnimationComponent"));
}

// Called when the game starts or when spawned
void ASmithAnimPawnTest::BeginPlay()
{
	Super::BeginPlay();

	//AGameOver* gameover = LoadObject<AGameOver>(nullptr, TEXT("/Game/TestLevel/ueno/GameOver_BP"));
	//if (gameover != nullptr)
	//{
	//	//gameover->GameOverEventDispatcher.AddActor(this, &AGameOver::TriggerdEvent);
	//}

}

// Called every frame
void ASmithAnimPawnTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SwitchAnimation(state);
}

// Called to bind functionality to input
void ASmithAnimPawnTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ASmithAnimPawnTest::SwitchAnimation(uint8 animationState)
{

	if (AnimationComp == nullptr)
	{
		return;
	}

	using namespace UE::Smith;
	FName StateName;
	switch (animationState)
	{
	case SMITH_ANIM_IDLE:
		StateName = TEXT("Idle");
		break;
	case	SMITH_ANIM_WALK:
		StateName = TEXT("Walk");
		break;
	case SMITH_ANIM_ATTACK:
		StateName = TEXT("Attack");
		break;
	case SMITH_ANIM_DAMAGED:
		StateName = TEXT("Damaged");
		break;
	case SMITH_ANIM_DEAD:
		StateName = TEXT("Dead");
		break;
	default:
		break;
	}
	AnimationComp->SwitchAnimState(StateName);
}

void ASmithAnimPawnTest::UpdateAnimation(float deltaTime)
{

}

void ASmithAnimPawnTest::SwitchAnimationDelay(uint8,float)
{
	
}

bool ASmithAnimPawnTest::IsAnimationFinish() const
{
	return false;
}

void ASmithAnimPawnTest::OpenGameOverUI_Implementation(){}


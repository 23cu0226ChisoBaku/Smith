// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithActors/Weapon/Weapon_testCharacter.h"
#include "SmithActors/Weapon/Private/Weapon.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon_testCharacter::AWeapon_testCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapon_testCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeapon_testCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWeapon_testCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Move", this, &AWeapon_testCharacter::GetInput);

}

void AWeapon_testCharacter::GetInput(float value)
{
	TSubclassOf<AActor>findClass;
	findClass = AActor::StaticClass();
	FName _tag;
	TArray<AActor*>actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), findClass, actors);

	// �������ʁAActor�������
	if (actors.Num() > 0)
	{
		// ����Actor�̒������ԂɌ���
		for (int idx = 0; idx < actors.Num(); idx++)
		{
			AActor* pActor = Cast<AActor>(actors[idx]);

			// �^�O���Ŕ��ʂ���
			// (���O�̓G�f�B�^��ɏd�������ꍇ�͓����̖��O���ς�邽�߁A���ɂȂ�Ȃ�)
			if (pActor->ActorHasTag(_tag))
			{
				// �o�͊m�F�p���b�Z�[�W
				FString message = FString("Founded Actor : ") + pActor->GetName();
				UE_LOG(LogTemp, Warning, TEXT("%S"), *message);



				//return pActor;
			}
		}
	}
}

void AWeapon_testCharacter::SetWeapon(Weapon* pWeapon)
{
	if (pWeapon != NULL)
	{
		p_weapon = pWeapon;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("�ʂ��"));
	}
}

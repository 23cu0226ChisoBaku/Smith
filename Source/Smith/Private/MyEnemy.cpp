// Fill out your copyright notice in the Description page of Project Settings.

#include "MyEnemy.h"


// Sets default values
AMyEnemy::AMyEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// �U��
void AMyEnemy::Attack()
{
	// �J�n�ʒu�ƏI���ʒu���`
	float rayLenth = 10.0f;
	FVector StartLocation = GetActorLocation();

	// ���E�㉺�̏I�_
	FVector EndLocation[4] = {
		StartLocation + FVector::ForwardVector * rayLenth,
		StartLocation + FVector::BackwardVector * rayLenth,
		StartLocation + FVector::RightVector * rayLenth,
		StartLocation + FVector::LeftVector * rayLenth
	};

	// Trace�ɕK�v�ȃp�����[�^
	FHitResult HitResult;  // �q�b�g���
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);  // �������g�𖳎�

	bool bHit;
	for (int i = 0; i < EndLocation->Length(); ++i)
	{
		// LineTrace�����s
		bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,       // �q�b�g����
			StartLocation,   // �J�n�ʒu
			EndLocation[i],  // �I���ʒu
			ECC_Visibility,  // �g�p����R���W�����`���l���i�����ł͎��o�I�ȃR���W�����j
			CollisionParams  // �R���W�����p�����[�^
		);
		//if (bHit && HitResult.GetActor() == AMyPlayerCharacter::StaticClass())
		//{

		//}
	}


	// �q�b�g�����ꍇ�̏���
	if (bHit)
	{
		// �q�b�g�����I�u�W�F�N�g�̏����擾
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			if (GEngine != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Player to Damage"), *HitActor->GetName());
			}
		}
	}
}


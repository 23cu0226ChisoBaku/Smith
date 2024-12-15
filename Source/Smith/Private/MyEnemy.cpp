// ���ꂩ���邱��
// �U���i�����I�ɂ̓C�P�Ă�j
// �U�������Ƃ��G�t�F�N�g�Ȃ�킩��₷�����o������
// �_���[�W���󂯂�
// �^�[�����ōU���i�܂�v���C���[���ړ�������U���j<-�Q�Ɛݒ�Ƃ��߂�ǂ����璇��N���X���o�R���Ă�낤����
// �v���C���[�ɑ΂��Ĉړ��i�}�X�ځj
// 
#include "MyEnemy.h"
#include "Kismet/KismetSystemLibrary.h"



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
	Attack();
}

// �U��
void AMyEnemy::Attack()
{
	// �J�n�ʒu�ƏI���ʒu���`
	const float rayLenth = 100.0f;
	const FVector StartLocation = GetActorLocation();

	// ���E�㉺�̏I�_
	const FVector EndLocation[4] = {
		StartLocation + FVector::ForwardVector * rayLenth,
		StartLocation + FVector::BackwardVector * rayLenth,
		StartLocation + FVector::RightVector * rayLenth,
		StartLocation + FVector::LeftVector * rayLenth
	};

	// Trace�ɕK�v�ȃp�����[�^
	FHitResult HitResult;  // �q�b�g���
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);  // �������g�𖳎�
	AActor* HitActor = nullptr;
	bool bHit = false;

	for (int i = 0; i < 4; ++i)
	{
		// LineTrace�����s
		bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,       // �q�b�g����
			StartLocation,   // �J�n�ʒu
			EndLocation[i],  // �I���ʒu
			ECC_PhysicsBody,  // �g�p����R���W�����`���l���i�����ł͕����I�ȃR���W�����j
			CollisionParams  // �R���W�����p�����[�^
		);

		if (!bHit)
		{
			continue;
		}

		HitActor = HitResult.GetActor();

		// �v���C���[�Ƀq�b�g�����ꍇ�̏���
		if (::IsValid(HitActor) && HitActor->IsA(AMyPlayerCharacter::StaticClass()))
		{
			// �q�b�g�����ʒu�ɐԐF�̓_��\��
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 1.0f);

			// ���C�L���X�g�̐���F�ŕ\��
			DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Blue, false, 1.0f, 0, 1.0f);

			if (GEngine != nullptr)
			{
				// �����Ńv���C���[�ɍU���I�I�I
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, HitActor->GetName());
				// PrintString�m�[�h�Ɠ�������
				// UKismetSystemLibrary�N���X��PrintString�֐����Ăяo��
				UKismetSystemLibrary::PrintString(this, "Player to Attack!", true, true, FColor::Cyan, 2.f, TEXT("None"));
			}
			return;
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		// �q�b�g���Ȃ������ꍇ�A���C�L���X�g���I�������ʒu�ɗΐF�̓_��\��
		DrawDebugPoint(GetWorld(), EndLocation[i], 10.0f, FColor::Green, false, 1.0f);

		// ���C�L���X�g�̐���ΐF�ŕ\��
		DrawDebugLine(GetWorld(), StartLocation, EndLocation[i], FColor::Green, false, 1.0f, 0, 1.0f);
	}
}


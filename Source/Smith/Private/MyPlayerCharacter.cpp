#include "MyPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
 	// Tick���g��
	PrimaryActorTick.bCanEverTick = true;

	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("m_pSpringArm"));
	if (m_pSpringArm != nullptr)
	{
		m_pSpringArm->SetupAttachment(RootComponent);

		// �A�[���̒����ݒ�
		// �J�����̃R���W�����e�X�g���s�����ݒ�
		m_pSpringArm->bDoCollisionTest = false;
		// �J�����̒Ǐ]���O���g������ݒ�
		m_pSpringArm->bEnableCameraLag = false;
		// �J������]���O���g������ݒ�
		m_pSpringArm->bEnableCameraRotationLag = false;
	}
	else
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("SpringArm Dangerous!!!"));
		}
	}

	// �J�����̃I�u�W�F�N�g��ݒ�
	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_pCamera"));
	if ((m_pCamera != nullptr) && (m_pSpringArm != NULL))
	{
		// �J�������X�v�����O�A�[���ɃA�^�b�`�ɂ�����
		m_pCamera->SetupAttachment(m_pSpringArm, USpringArmComponent::SocketName);
	}

	else
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Camera Dangerous!!!"));
		}

	}
}

// Called when the game starts or when spawned
void AMyPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePawn();
}

// ���͂ƃo�C���h����
void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn�̈ړ�
	InputComponent->BindAxis("MoveForward", this, &AMyPlayerCharacter::Pawn_MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyPlayerCharacter::Pawn_MoveRight);
}

void AMyPlayerCharacter::UpdatePawn()
{
	 //�ړ����͂��Ȃ������ꍇ����return
	if (m_pos.IsZero())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Return"));
		return;
	}
	//���݂̍��W
	FVector pos = GetActorLocation();
	pos += m_pos;
	// ���W�Ɖ�]�Z�b�g
	SetActorLocationAndRotation(pos, m_dir);
	// �ړ��ʂ��O�ɖ߂�
	m_pos = FVector::ZeroVector;
}

// �O��ړ�
void AMyPlayerCharacter::Pawn_MoveForward(float axisValue)
{
	m_pos.X += (axisValue * 5.0f);
	m_dir.Roll = (axisValue == 1.0f ? 0.0f : 180.0f);
}

// ���E�ړ�
void AMyPlayerCharacter::Pawn_MoveRight(float axisValue)
{
	m_pos.Y += (axisValue * 5.0f);
	m_dir.Roll = 90.0f * axisValue;
}
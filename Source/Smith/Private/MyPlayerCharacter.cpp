#include "MyPlayerCharacter.h"

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	// �ړ����͂��Ȃ������ꍇ����return
	if (m_pawnMoveInput.IsZero())
	{
		return;
	}
	m_pawnMoveInput = m_pawnMoveInput.GetSafeNormal() * 50.0f;
	FVector NewLocation = GetActorLocation();

	// Actor�������Ă�������x�N�g�����擾���A�����ɓ��͒l��������
	NewLocation += GetActorForwardVector() * m_pawnMoveInput.Y;
	NewLocation += GetActorRightVector() * m_pawnMoveInput.X;
	SetActorLocation(NewLocation);
}

// �O��ړ�
void AMyPlayerCharacter::Pawn_MoveForward(float axisValue)
{
	m_pawnMoveInput.Y = FMath::Clamp(axisValue, -1.0f, 1.0f);
}

// ���E�ړ�
void AMyPlayerCharacter::Pawn_MoveRight(float axisValue)
{
	m_pawnMoveInput.X = FMath::Clamp(axisValue, -1.0f, 1.0f);
}
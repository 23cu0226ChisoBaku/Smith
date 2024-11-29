#include "MyPlayerCharacter.h"

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
 	// Tick���g��
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
	 //�ړ����͂��Ȃ������ꍇ����return
	if (m_pos.IsZero())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Return"));
		return;
	}
	//���݂̍��W
	FVector pos = GetActorLocation();
	pos += m_pos;
	// ���W���Z�b�g
	SetActorLocation(pos);
	// �O�ɖ߂�
	m_pos = FVector::ZeroVector;
}

// �O��ړ�
void AMyPlayerCharacter::Pawn_MoveForward(float axisValue)
{
	m_pos.X += (axisValue * 5.0f);
}

// ���E�ړ�
void AMyPlayerCharacter::Pawn_MoveRight(float axisValue)
{
	m_pos.Y += (axisValue * 5.0f);
}
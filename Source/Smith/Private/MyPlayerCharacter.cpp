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

// 入力とバインド処理
void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawnの移動
	InputComponent->BindAxis("MoveForward", this, &AMyPlayerCharacter::Pawn_MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyPlayerCharacter::Pawn_MoveRight);
}

void AMyPlayerCharacter::UpdatePawn()
{
	// 移動入力がなかった場合早期return
	if (m_pawnMoveInput.IsZero())
	{
		return;
	}
	m_pawnMoveInput = m_pawnMoveInput.GetSafeNormal() * 50.0f;
	FVector NewLocation = GetActorLocation();

	// Actorが向いている方向ベクトルを取得し、そこに入力値をかける
	NewLocation += GetActorForwardVector() * m_pawnMoveInput.Y;
	NewLocation += GetActorRightVector() * m_pawnMoveInput.X;
	SetActorLocation(NewLocation);
}

// 前後移動
void AMyPlayerCharacter::Pawn_MoveForward(float axisValue)
{
	m_pawnMoveInput.Y = FMath::Clamp(axisValue, -1.0f, 1.0f);
}

// 左右移動
void AMyPlayerCharacter::Pawn_MoveRight(float axisValue)
{
	m_pawnMoveInput.X = FMath::Clamp(axisValue, -1.0f, 1.0f);
}
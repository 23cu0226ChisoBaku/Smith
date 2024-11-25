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

}

// ì¸óÕÇ∆ÉoÉCÉìÉhèàóù
void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn
}

void AMyPlayerCharacter::UpdatePawn()
{

}

void AMyPlayerCharacter::Pawn_MoveForward(float axisValue)
{
	m_pawnMoveInput.Y = FMath::Clamp(axisValue, -1.0f, -1.0f);
}

void AMyPlayerCharacter::Pawn_MoveRight(float axisValue)
{
	m_pawnMoveInput.X = FMath::Clamp(axisValue, -1.0f, -1.0f);
}




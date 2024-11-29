#include "MyPlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h" // �ǉ�
#include "Kismet/GameplayStatics.h" // �ǉ�
#include "Components/InputComponent.h" // �ǉ�
#include "EnhancedInputComponent.h" // �ǉ�
#include "EnhancedInputSubsystems.h" // �ǉ�

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{

	// Input Action��Ǎ�
	ActionInput = LoadObject<UInputAction>(NULL, TEXT("/Game/EnhancedInputTest/Input/Actions/IA_Action"), NULL, LOAD_None, NULL);
	AxisInput = LoadObject<UInputAction>(NULL, TEXT("/Game/EnhancedInputTest/Input/Actions/IA_Axis"), NULL, LOAD_None, NULL);

	// Input Mapping Context��Ǎ�
	DefaultMappingContext = LoadObject<UInputMappingContext>(NULL, TEXT("/Game/EnhancedInputTest/Input/IMC_Default"), NULL, LOAD_None, NULL);

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
	// �ړ����͂��Ȃ������ꍇ����return
	//if (m_pawnMoveInput.IsZero())
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Return"));
	//	return;
	//}
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Move"));

	FVector pos = GetActorLocation();

	SetActorLocation(pos);
}

// �O��ړ�
void AMyPlayerCharacter::Pawn_MoveForward(float axisValue)
{
	if (GEngine != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("MoveForward"));
	}
}

// ���E�ړ�
void AMyPlayerCharacter::Pawn_MoveRight(float axisValue)
{
	if (GEngine != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("�����Ă��ꂦ������"));
	}
}
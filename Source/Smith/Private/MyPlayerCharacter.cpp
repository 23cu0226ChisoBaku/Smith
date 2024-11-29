#include "MyPlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h" // 追加
#include "Kismet/GameplayStatics.h" // 追加
#include "Components/InputComponent.h" // 追加
#include "EnhancedInputComponent.h" // 追加
#include "EnhancedInputSubsystems.h" // 追加

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{

	// Input Actionを読込
	ActionInput = LoadObject<UInputAction>(NULL, TEXT("/Game/EnhancedInputTest/Input/Actions/IA_Action"), NULL, LOAD_None, NULL);
	AxisInput = LoadObject<UInputAction>(NULL, TEXT("/Game/EnhancedInputTest/Input/Actions/IA_Axis"), NULL, LOAD_None, NULL);

	// Input Mapping Contextを読込
	DefaultMappingContext = LoadObject<UInputMappingContext>(NULL, TEXT("/Game/EnhancedInputTest/Input/IMC_Default"), NULL, LOAD_None, NULL);

 	// Tickを使う
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
	//if (m_pawnMoveInput.IsZero())
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Return"));
	//	return;
	//}
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Move"));

	FVector pos = GetActorLocation();

	SetActorLocation(pos);
}

// 前後移動
void AMyPlayerCharacter::Pawn_MoveForward(float axisValue)
{
	if (GEngine != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("MoveForward"));
	}
}

// 左右移動
void AMyPlayerCharacter::Pawn_MoveRight(float axisValue)
{
	if (GEngine != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("動いてくれええええ"));
	}
}
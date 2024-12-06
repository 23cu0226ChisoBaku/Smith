#include "MyPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
 	// Tickを使う
	PrimaryActorTick.bCanEverTick = true;

	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("m_pSpringArm"));
	if (m_pSpringArm != nullptr)
	{
		m_pSpringArm->SetupAttachment(RootComponent);

		// アームの長さ設定
		// カメラのコリジョンテストを行うか設定
		m_pSpringArm->bDoCollisionTest = false;
		// カメラの追従ラグを使うかを設定
		m_pSpringArm->bEnableCameraLag = false;
		// カメラ回転ラグを使うかを設定
		m_pSpringArm->bEnableCameraRotationLag = false;
	}
	else
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("SpringArm Dangerous!!!"));
		}
	}

	// カメラのオブジェクトを設定
	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_pCamera"));
	if ((m_pCamera != nullptr) && (m_pSpringArm != NULL))
	{
		// カメラをスプリングアームにアタッチにさせる
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
	 //移動入力がなかった場合早期return
	if (m_pos.IsZero())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Return"));
		return;
	}
	//現在の座標
	FVector pos = GetActorLocation();
	pos += m_pos;
	// 座標と回転セット
	SetActorLocation(pos);
	// 移動量を０に戻す
	m_pos = FVector::ZeroVector;
}

// 前後移動
void AMyPlayerCharacter::Pawn_MoveForward(float axisValue)
{
	m_pos.X += (axisValue * 5.0f);
}

// 左右移動
void AMyPlayerCharacter::Pawn_MoveRight(float axisValue)
{
	m_pos.Y += (axisValue * 5.0f);
}
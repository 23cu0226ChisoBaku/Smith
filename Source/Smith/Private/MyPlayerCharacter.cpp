#include "MyPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
 	// Tick�ｽ�ｽ�ｽg�ｽ�ｽ
	PrimaryActorTick.bCanEverTick = true;

	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("m_pSpringArm"));
	if (m_pSpringArm != nullptr)
	{
		m_pSpringArm->SetupAttachment(RootComponent);

		// �ｽA�ｽ[�ｽ�ｽ�ｽﾌ抵ｿｽ�ｽ�ｽ�ｽﾝ抵ｿｽ
		// �ｽJ�ｽ�ｽ�ｽ�ｽ�ｽﾌコ�ｽ�ｽ�ｽW�ｽ�ｽ�ｽ�ｽ�ｽe�ｽX�ｽg�ｽ�ｽ�ｽs�ｽ�ｽ�ｽ�ｽ�ｽﾝ抵ｿｽ
		m_pSpringArm->bDoCollisionTest = false;
		// �ｽJ�ｽ�ｽ�ｽ�ｽ�ｽﾌ追従�ｽ�ｽ�ｽO�ｽ�ｽ�ｽg�ｽ�ｽ�ｽ�ｽ�ｽ�ｽﾝ抵ｿｽ
		m_pSpringArm->bEnableCameraLag = false;
		// �ｽJ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ]�ｽ�ｽ�ｽO�ｽ�ｽ�ｽg�ｽ�ｽ�ｽ�ｽ�ｽ�ｽﾝ抵ｿｽ
		m_pSpringArm->bEnableCameraRotationLag = false;
	}
	else
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("SpringArm Dangerous!!!"));
		}
	}

	// �ｽJ�ｽ�ｽ�ｽ�ｽ�ｽﾌオ�ｽu�ｽW�ｽF�ｽN�ｽg�ｽ�ｽﾝ抵ｿｽ
	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_pCamera"));
	if ((m_pCamera != nullptr) && (m_pSpringArm != NULL))
	{
		// �ｽJ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽX�ｽv�ｽ�ｽ�ｽ�ｽ�ｽO�ｽA�ｽ[�ｽ�ｽ�ｽﾉア�ｽ^�ｽb�ｽ`�ｽﾉゑｿｽ�ｽ�ｽ�ｽ�ｽ
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

// �ｽ�ｽ�ｽﾍとバ�ｽC�ｽ�ｽ�ｽh�ｽ�ｽ�ｽ�ｽ
void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn�ｽﾌ移難ｿｽ
	InputComponent->BindAxis("MoveForward", this, &AMyPlayerCharacter::Pawn_MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyPlayerCharacter::Pawn_MoveRight);
}

void AMyPlayerCharacter::UpdatePawn()
{
	 //�ｽﾚ難ｿｽ�ｽ�ｽ�ｽﾍゑｿｽ�ｽﾈゑｿｽ�ｽ�ｽ�ｽ�ｽ�ｽ鼾�ｿｽ�ｽ�ｽ�ｽreturn
	if (m_pos.IsZero())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Return"));
		return;
	}
	//�ｽ�ｽ�ｽﾝの搾ｿｽ�ｽW
	FVector pos = GetActorLocation();
	pos += m_pos;

	// 座標と回転セット
	SetActorLocation(pos);
	// 移動量を０に戻す
	m_pos = FVector::ZeroVector;
}

// �ｽO�ｽ�ｽﾚ難ｿｽ
void AMyPlayerCharacter::Pawn_MoveForward(float axisValue)
{
	m_pos.X += (axisValue * 5.0f);
}

// �ｽ�ｽ�ｽE�ｽﾚ難ｿｽ
void AMyPlayerCharacter::Pawn_MoveRight(float axisValue)
{
	m_pos.Y += (axisValue * 5.0f);
}
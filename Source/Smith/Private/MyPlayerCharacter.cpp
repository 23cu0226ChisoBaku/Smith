#include "MyPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h" // 追加
#include "Kismet/GameplayStatics.h" // 追加
#include "Components/InputComponent.h" // 追加
#include "EnhancedInputComponent.h" // 追加
#include "EnhancedInputSubsystems.h" // 追加

// 結局インプットよくわからん

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("m_pSpringArm"));
	if (m_pSpringArm != nullptr)
	{
		m_pSpringArm->SetupAttachment(RootComponent);

		m_pSpringArm->bEnableCameraLag = false;
		m_pSpringArm->bEnableCameraRotationLag = false;
	}
	else
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("SpringArm Dangerous!!!"));
		}
	}

	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_pCamera"));
	if ((m_pCamera != nullptr) && (m_pSpringArm != NULL))
	{
		m_pCamera->SetupAttachment(m_pSpringArm, USpringArmComponent::SocketName);
	}

	else
	{
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Camera Dangerous!!!"));
		}
	}
	// Input Actionを読込
	ActionInput = LoadObject<UInputAction>(NULL, TEXT("/Game/EnhancedInputTest/Input/Actions/IA_Action"), NULL, LOAD_None, NULL);
	AxisInput = LoadObject<UInputAction>(NULL, TEXT("/Game/EnhancedInputTest/Input/Actions/IA_Axis"), NULL, LOAD_None, NULL);

	// Input Mapping Contextを読込
	DefaultMappingContext = LoadObject<UInputMappingContext>(NULL, TEXT("/Game/EnhancedInputTest/Input/IMC_Default"), NULL, LOAD_None, NULL);
	isMove = true;
}

// Called when the game starts or when spawned
void AMyPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

		// Input設定を行う
	SetupInput();	
}

void AMyPlayerCharacter::SetupInput()
{
	// PlayerControllerを取得する
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// 入力を有効にする
	EnableInput(controller);

	if (InputComponent)
	{
		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {

			// Input Action
			EnhancedInputComponent->BindAction(ActionInput, ETriggerEvent::Triggered, this, &AMyPlayerCharacter::PressedAction);
			EnhancedInputComponent->BindAction(ActionInput, ETriggerEvent::Completed, this, &AMyPlayerCharacter::ReleasedAction);

			// Input Axis
			EnhancedInputComponent->BindAction(AxisInput, ETriggerEvent::Triggered, this, &AMyPlayerCharacter::PressedAxis);
		}

		// Input Mapping Contextを登録する
		if (APlayerController* PlayerController = Cast<APlayerController>(controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void AMyPlayerCharacter::PressedAction()
{
	if (!IsPressed)
	{
		// Pressed
		UKismetSystemLibrary::PrintString(this, TEXT("Pressed"), true, true, FColor::Cyan, 10.0f, TEXT("None"));

		// Press状態に設定
		IsPressed = true;
	}
}

void AMyPlayerCharacter::ReleasedAction()
{
	// Released
	UKismetSystemLibrary::PrintString(this, TEXT("Released"), true, true, FColor::Cyan, 10.0f, TEXT("None"));

	// Press状態を解除
	IsPressed = false;
}

void AMyPlayerCharacter::PressedAxis(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D v = Value.Get<FVector2D>();

	// Axis Input Value
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("X:%f Y:%f"), v.X, v.Y), true, true, FColor::Cyan, 10.0f, TEXT("None"));
}

void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 入力をフレーム毎に監視
    PlayerInputComponent->BindKey(EKeys::W, IE_Pressed, this, &AMyPlayerCharacter::Pawn_MoveForward);
	PlayerInputComponent->BindKey(EKeys::S, IE_Pressed, this, &AMyPlayerCharacter::Pawn_MoveBackward);
	PlayerInputComponent->BindKey(EKeys::A, IE_Pressed, this, &AMyPlayerCharacter::Pawn_MoveLeft);
	PlayerInputComponent->BindKey(EKeys::D, IE_Pressed, this, &AMyPlayerCharacter::Pawn_MoveRight);
}
// Called every frame
void AMyPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isMove)
	{
		moveTimer += DeltaTime;
		if (moveTimer > 1.0f)
		{
			isMove = true;
			moveTimer = 0.0f;
		}
	}
}


void AMyPlayerCharacter::UpdatePawn()
{
	FVector pos = GetActorLocation();
	pos += m_pos;

	// 座標と回転セット
	SetActorLocation(pos);
	// 移動量を０に戻す
	m_pos = FVector::ZeroVector;

	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Player Move"));
	}
}

void AMyPlayerCharacter::Pawn_MoveForward()
{
	if (isMove)
	{
		isMove = false;
		m_pos.X +=  MOVE_DISTANCE;
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Input W"));
		}
		UpdatePawn();
	}
}

void AMyPlayerCharacter::Pawn_MoveBackward()
{
	if (isMove)
	{
		isMove = false;
		m_pos.X -=  MOVE_DISTANCE;
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Input S"));
		}
		UpdatePawn();
	}
}

void AMyPlayerCharacter::Pawn_MoveRight()
{
	if (isMove)
	{
		isMove = false;
		m_pos.Y +=  MOVE_DISTANCE;
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Input D"));
		}
		UpdatePawn();
	}
}

void AMyPlayerCharacter::Pawn_MoveLeft()
{
	if (isMove)
	{
		isMove = false;
		m_pos.Y -=  MOVE_DISTANCE;
		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Input A"));
		}
		UpdatePawn();
	}
}
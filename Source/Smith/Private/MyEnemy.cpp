// Fill out your copyright notice in the Description page of Project Settings.

#include "MyEnemy.h"


// Sets default values
AMyEnemy::AMyEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 攻撃
void AMyEnemy::Attack()
{
	// 開始位置と終了位置を定義
	float rayLenth = 10.0f;
	FVector StartLocation = GetActorLocation();

	// 左右上下の終点
	FVector EndLocation[4] = {
		StartLocation + FVector::ForwardVector * rayLenth,
		StartLocation + FVector::BackwardVector * rayLenth,
		StartLocation + FVector::RightVector * rayLenth,
		StartLocation + FVector::LeftVector * rayLenth
	};

	// Traceに必要なパラメータ
	FHitResult HitResult;  // ヒット情報
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);  // 自分自身を無視

	bool bHit;
	for (int i = 0; i < EndLocation->Length(); ++i)
	{
		// LineTraceを実行
		bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,       // ヒット結果
			StartLocation,   // 開始位置
			EndLocation[i],  // 終了位置
			ECC_Visibility,  // 使用するコリジョンチャネル（ここでは視覚的なコリジョン）
			CollisionParams  // コリジョンパラメータ
		);
		//if (bHit && HitResult.GetActor() == AMyPlayerCharacter::StaticClass())
		//{

		//}
	}


	// ヒットした場合の処理
	if (bHit)
	{
		// ヒットしたオブジェクトの情報を取得
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			if (GEngine != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, TEXT("Player to Damage"), *HitActor->GetName());
			}
		}
	}
}


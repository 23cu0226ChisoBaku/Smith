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
	Attack();
}

// 攻撃
void AMyEnemy::Attack()
{
	// 開始位置と終了位置を定義
	const float rayLenth = 100.0f;
	const FVector StartLocation = GetActorLocation();

	// 左右上下の終点
	const FVector EndLocation[4] = {
		StartLocation + FVector::ForwardVector * rayLenth,
		StartLocation + FVector::BackwardVector * rayLenth,
		StartLocation + FVector::RightVector * rayLenth,
		StartLocation + FVector::LeftVector * rayLenth
	};

	// Traceに必要なパラメータ
	FHitResult HitResult;  // ヒット情報
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);  // 自分自身を無視
	AActor* HitActor = nullptr;
	bool bHit = false;

	for (int i = 0; i < 4; ++i)
	{
		// LineTraceを実行
		bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,       // ヒット結果
			StartLocation,   // 開始位置
			EndLocation[i],  // 終了位置
			ECC_PhysicsBody,  // 使用するコリジョンチャネル（ここでは物理的なコリジョン）
			CollisionParams  // コリジョンパラメータ
		);

		if (!bHit)
		{
			continue;
		}

		HitActor = HitResult.GetActor();

		// プレイヤーにヒットした場合の処理
		if (::IsValid(HitActor) && HitActor->IsA(AMyPlayerCharacter::StaticClass()))
		{
			// ヒットした位置に赤色の点を表示
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 1.0f);

			// レイキャストの線を青色で表示
			DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Blue, false, 1.0f, 0, 1.0f);

			if (GEngine != nullptr)
			{
				// ここでプレイヤーに攻撃！！！
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Player to Attack");
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, HitActor->GetName());
			}
			return;
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		// ヒットしなかった場合、レイキャストが終了した位置に緑色の点を表示
		DrawDebugPoint(GetWorld(), EndLocation[i], 10.0f, FColor::Green, false, 1.0f);

		// レイキャストの線を緑色で表示
		DrawDebugLine(GetWorld(), StartLocation, EndLocation[i], FColor::Green, false, 1.0f, 0, 1.0f);
	}
}


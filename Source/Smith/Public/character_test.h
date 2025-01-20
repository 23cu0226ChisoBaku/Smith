// mapObjectの動作確認用Characterクラス
// こいつにMapObjectをくっつけて色々試す

#pragma once

#include "MapObject.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "character_test.generated.h"


UCLASS()
class SMITH_API Acharacter_test : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Acharacter_test();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void move(float);
	void UpdateMove();

	MapObject mapObj;
};

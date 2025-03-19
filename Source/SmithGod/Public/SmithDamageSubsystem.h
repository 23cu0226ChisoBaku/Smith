// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "BattleParamHandle.h"
#include "BattleResult.h"

#include "SmithDamageSubsystem.generated.h"

class USmithBattleLogWorldSubsystem;
struct FAttackHandle;
enum class EDirection : uint8;


/**
 * @brief バトルを制御する
 */
UCLASS()
class SMITHGOD_API USmithDamageSubsystem final: public UWorldSubsystem
{
	GENERATED_BODY()

private:

	// ダメージ計算インターフェース
	struct IDamageCalculationConcept
	{
		virtual FBattleResult CalculateDamage(FBattleAttackParamHandle AttackParamHandle, FBattleDefenseParamHandle DefenseParamHandle) const = 0;
		virtual ~IDamageCalculationConcept() {};
	};

	// ダメージ計算ストラテジーモデル
	// typename DamageCalculationStrategy : ストラテジー
	template<typename DamageCalculationStrategy>
	struct DamageCalculationModel : public IDamageCalculationConcept
	{
		DamageCalculationModel(DamageCalculationStrategy Strategy_)
			: Strategy(Strategy_)
		{ }

		FBattleResult CalculateDamage(FBattleAttackParamHandle AttackParamHandle, FBattleDefenseParamHandle DefenseParamHandle) const override
		{
			return Strategy(AttackParamHandle, DefenseParamHandle);
		}

		DamageCalculationStrategy Strategy;
	};

public:

	//---Begin of USubsystem Interface
	bool ShouldCreateSubsystem(UObject* Outer) const override final;
	void Initialize(FSubsystemCollectionBase& Collection) override final;
	void Deinitialize() override final;
	//---End of USubsystem Interface

	// ダメージ計算ストラテジーを入れ替える
	template<typename DamageCalculationStrategy>
	void SetDamageStrategy(DamageCalculationStrategy Strategy)
	{
		m_pimpl.Reset();
		m_pimpl = ::MakeUnique<DamageCalculationModel<DamageCalculationStrategy>>(Strategy);
	}

	// ダメージ計算を行う
	void ApplyDamage(AActor* DamageInstigator, AActor* DamageCauser, const FAttackHandle& Handle, EDirection FromDirection) const;

private:

	TUniquePtr<IDamageCalculationConcept> m_pimpl;

	TWeakObjectPtr<USmithBattleLogWorldSubsystem> m_logSystem;
};

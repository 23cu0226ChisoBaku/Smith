// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_BATTLE_LOG_MODEL
#define SMITH_BATTLE_LOG_MODEL

class USmithBattleLogModelDefinition;
enum class EBattleLogModelType : uint8;

/// @brief バトルログモデル
struct SMITHMODEL_API FSmithBattleLogModel
{
	friend class USmithBattleLogModelRepository;

	private:

		FSmithBattleLogModel() = default;

		static FSmithBattleLogModel CreateModel(USmithBattleLogModelDefinition* LogDefinition);

	public:

		FString GetName() const;

		EBattleLogModelType GetType() const;

		bool IsValid() const;

	private:

		TWeakObjectPtr<USmithBattleLogModelDefinition> m_definition;

};

#endif
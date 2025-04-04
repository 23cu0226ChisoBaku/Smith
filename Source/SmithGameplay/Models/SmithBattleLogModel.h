// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_BATTLE_LOG_MODEL
#define SMITH_BATTLE_LOG_MODEL

#include "BattleLogType.h"

/// @brief バトルログモデル
struct SMITHGAMEPLAY_API FSmithBattleLogModel
{
	friend class USmithBattleLogModelRepository;

	private:

		FSmithBattleLogModel();

		static FSmithBattleLogModel CreateModel(UClass* LogModelClass, const FString& Name, EBattleLogModelType Type);

		UClass* GetModelOwnerClass() const
		{
			return m_logModelClass;
		}

	public:

		FString GetName() const
		{
			return m_name;
		}

		EBattleLogModelType GetType() const
		{
			return m_type;
		}

		bool IsValid() const;

	private:

		TSubclassOf<UObject> m_logModelClass;
		FString m_name;
		EBattleLogModelType m_type;

};

#endif
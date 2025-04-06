// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_EVENT_MODEL
#define SMITH_EVENT_MODEL

/// @brief イベントモデル
struct SMITHGAMEPLAY_API FSmithEventModel
{
	friend class USmithEventModelRepository;
	
	private:

		FSmithEventModel();

		static FSmithEventModel CreateModel(UClass* EventClass, const FString& SucceededMsg, const FString& FailedMsg);

	public:

		FString GetSucceededMsg() const
		{
			return m_succeededMsg;
		}

		FString GetFailedMsg() const
		{
			return m_failedMsg;
		}

		bool IsValid() const;

	private:

		TSubclassOf<UObject> m_eventClass;
		FString m_succeededMsg;
		FString m_failedMsg;
};

#endif

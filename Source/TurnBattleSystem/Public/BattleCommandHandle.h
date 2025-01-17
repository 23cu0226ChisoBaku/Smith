// // Fill out your copyright notice in the Description page of Project Settings.

// #pragma once

// #include "CoreMinimal.h"
// #include "UObject/WeakInterfacePtr.h"
// #include "ITurnManageable.h"
// #include "IBattleCommand.h"

// class SMITH_API BattleCommandHandle
// {

// public:
//   bool IsValid() const;
//   bool IsCommandValid() const;
//   bool IsRequesterValid() const;

//   template<typename Type>
//   Type* GetRequester() const;

//   template<typename Type>
//   Type* GetCommand() const;

//   template<typename Type>
//   bool IsRequestImplemented();

//   template<typename Type>
//   bool IsCommandImplemented();
  
// private:
//   TWeakInterfacePtr<ITurnManageable> m_requester;
//   TWeakPtr<IBattleCommand> m_command;
// }
#pragma once

#ifndef SMITH_BATTLE_HEAL_COMMAND
#define SMITH_BATTLE_HEAL_COMMAND

#include "CoreMinimal.h"
#include "IBattleCommand.h"

class IHealable;

namespace UE::Smith::Command
{
  ///
  ///	@brief ヒールコマンド
  /// namespace UE::Smith::Command
  /// implemented IBattleCommand
  ///
  class SMITH_API HealCommand final: public IBattleCommand
  {
    public:
      HealCommand(IHealable*);
      ~HealCommand();

    public:
      void Start() override final;
      void Execute(float deltaTime) override final;
      void End() override final;
      bool IsFinish() const override final;

      private:
      /// @brief	移動コマンド実装クラス(pImplイディオム)
      class HealImpl;
      TUniquePtr<HealImpl> m_healImpl;
  };
}

#endif
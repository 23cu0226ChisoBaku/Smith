

#pragma once

#ifndef SMITH_BATTLE_COMMAND
#define SMITH_BATTLE_COMMAND

#include "Templates/UniquePtr.h"
#include "BattleCommandStrategies.h"

namespace UE::Smith::Command
{
  class BattleCommand
  {
    private:
      struct CommandExecutor
      {
        virtual ~CommandExecutor() = default;

        virtual void DoStart() const = 0;
        virtual void DoExecute(float DeltaTime) const = 0;
        virtual void DoEnd() const = 0;
        virtual bool CanExecuteFinish() const = 0;
      };

      template<typename CommandType>
      struct CommandModel : public CommandExecutor
      {
        CommandModel(CommandType Cmd)
          : CmdEntity(::MoveTemp(Cmd))
        {
        }

        void DoStart() const override
        {
          StartCommand(CmdEntity);
        }

        void DoExecute(float DeltaTime) const override
        {
          ExecuteCommand(CmdEntity, DeltaTime);
        }

        void DoEnd() const override
        {
          EndCommand(CmdEntity);
        }

        bool CanExecuteFinish() const override
        {
          return true;
        }

        CommandType CmdEntity;
      };

    public:
      template<typename CommandType>
      explicit BattleCommand(CommandType Cmd)
        : m_pimpl(::MakeUnique<CommandModel<CommandType>>(::MoveTemp(Cmd)))
      {}

      virtual ~BattleCommand()
      {
        m_pimpl.Reset();
      }

      BattleCommand(BattleCommand&&) = default;
      BattleCommand& operator=(BattleCommand&&) = default;

      friend SMITH_API void StartCommand(const BattleCommand& Cmd)
      {
        Cmd.m_pimpl->DoStart();
      }

      friend SMITH_API void UpdateCommand(const BattleCommand& Cmd, float DeltaTime)
      {
        Cmd.m_pimpl->DoExecute(DeltaTime);
      }

      friend SMITH_API void TerminateCommand(const BattleCommand& Cmd)
      {
        Cmd.m_pimpl->DoEnd();
      }

    private:
      TUniquePtr<CommandExecutor> m_pimpl;

    private:
      BattleCommand(const BattleCommand&) = delete;
      BattleCommand& operator=(const BattleCommand&) = delete;
  };
}

#endif
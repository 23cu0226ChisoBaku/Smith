  // Fill out your copyright notice in the Description page of Project Settings.

  #pragma once

  #include "IBattleCommand.h"

  #include <concepts>
  #include <type_traits>


template<typename CommandEntityType>
concept CommandConcepts = requires(CommandEntityType CommandEntity, float DeltaTime)
{
  CommandEntity.Start();
  CommandEntity.Update(DeltaTime);
  CommandEntity.End();
  {CommandEntity.IsFinished()} -> std::convertible_to<bool>;
};

class FSmithBattleCommand : public IBattleCommand
{
  private:
    using ThisClass = typename FSmithBattleCommand;

    struct BattleCommandConcept
    {
      virtual ~BattleCommandConcept() = 0 {};

      virtual void OnStart() = 0;
      virtual void OnUpdate(float DeltaTime) = 0;
      virtual void OnEnd() = 0;
      virtual bool IsFinished() const = 0;
    };

    // コマンド実体テンプレートを隠蔽する内部クラス(Type Erasure)
    template<CommandConcepts CommandEntityType>
    struct BattleCommandModel : public BattleCommandConcept
    {
      public:
        BattleCommandModel(CommandEntityType CommandEntity_)
          : CommandEntity(::MoveTemp(CommandEntity_))
        {}

        ~BattleCommandModel() = default;

        void OnStart() override final
        {
          CommandEntity.Start();
        }

        void OnUpdate(float DeltaTime) override final
        {
          CommandEntity.Update(DeltaTime);
        }

        void OnEnd() override final
        {
          CommandEntity.End();
        }

        bool IsFinished() const override final
        {
          return CommandEntity.IsFinished();
        }

        CommandEntityType CommandEntity;
    };

  public:

    template<typename CommandEntityType>
    FSmithBattleCommand(CommandEntityType CommandEntity)
      : m_pimpl(::MakeUnique<BattleCommandModel<CommandEntityType>>(CommandEntity))
    {}

    ~FSmithBattleCommand()
    {
      m_pimpl.Reset();
    }

    FSmithBattleCommand(const ThisClass& other) = delete;
    ThisClass& operator=(const ThisClass& other) = delete;
    FSmithBattleCommand(ThisClass&& other) noexcept = default;
    ThisClass& operator=(ThisClass&& other) noexcept = default;

  public:

    //---Begin of IBattleCommand Interfaces
    void Start() override final
    {
      m_pimpl->OnStart();
    }

    void Execute(float DeltaTime) override final
    {
      m_pimpl->OnUpdate(DeltaTime);
    }

    void End() override final
    {
      m_pimpl->OnEnd();
    }

    bool IsFinish() const override final
    {
      return m_pimpl->IsFinished();
    }
    //---End of IBattleCommand Interfaces

  private:

    TUniquePtr<BattleCommandConcept> m_pimpl; 
};	



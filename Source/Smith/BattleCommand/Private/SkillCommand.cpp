// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillCommand.h"
// #include "SmithMoveComponent.h"
#include "ISkillable.h"
#include "UObject/WeakInterfacePtr.h"

#include "Debug.h"

namespace UE::Smith::Command
{
// SkillCommand実装
// SkillCommand PImpl
#pragma region SkillCommand PImpl
  class SkillCommand::SkillImpl
  {
  public:
    SkillImpl(ISkillable *skillable)
        : m_skillable(skillable)
    {
    }
    ~SkillImpl()
    {
      m_skillable.Reset();
    }

  public:
    void Start()
    {
    }
    void Update(float deltaTime)
    {
      FString skillStr{};
      if (m_skillable.IsValid())
      {
        skillStr.Append(m_skillable->_getUObject()->GetName());
        m_skillable->OnSkill();
      }
      else
      {
        skillStr.Append(TEXT("EMPTY OBJECT"));
      }

      skillStr.Append(TEXT(" Skill Command"));
      MDebug::Log(skillStr);
    }
    void End()
    {
    }
    bool IsFinish() const
    {
      return true;
    }

  private:
    TWeakInterfacePtr<ISkillable> m_skillable;
  };
#pragma endregion MoveCommand PImpl
  // end of MoveCommand PImpl

  SkillCommand::SkillCommand(ISkillable *skillable)
      : m_skillImpl(new SkillImpl(skillable))
  {
  }

  SkillCommand::~SkillCommand()
  {
    m_skillImpl.Reset();
    memset(this, 0, sizeof(*this));
  }

  void SkillCommand::Start()
  {
    m_skillImpl->Start();
  }

  void SkillCommand::Execute(float deltaTime)
  {
    m_skillImpl->Update(deltaTime);
  }

  void SkillCommand::End()
  {
    m_skillImpl->End();
  }

  bool SkillCommand::IsFinish() const
  {
    return m_skillImpl->IsFinish();
  }
}
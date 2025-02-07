// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveCommand.h"
#include "SmithPlayerActor.h"
#include "SmithMoveComponent.h"
#include "IMoveable.h"
#include "ISmithAnimator.h"
#include "UObject/WeakInterfacePtr.h"

#include "Debug.h"

namespace UE::Smith::Command
{
  // MoveCommand実装
  // MoveCommand PImpl
  #pragma region MoveCommand PImpl
  class MoveCommand::MoveImpl
  {
    public:
      MoveImpl(IMoveable* moveable, ISmithAnimator* animator)
        : m_moveable(moveable)
        , m_animator(animator)
      { }
      ~MoveImpl()
      {
        m_moveable.Reset();
        m_animator.Reset();
      }
    public:
      void Start()
      {
        if (m_animator.IsValid())
        {
          m_animator->SwitchAnimation(SMITH_ANIM_WALK);
        } 
      }
      void Update(float deltaTime)
      {
        if (m_moveable.IsValid())
        {
          m_moveable->Move(deltaTime);
        }

        if (m_animator.IsValid())
        {
          m_animator->UpdateAnimation(deltaTime);
        }
      }
      void End()
      {
        if (m_animator.IsValid())
        {
          m_animator->SwitchAnimationDelay(SMITH_ANIM_IDLE, 0.2f);
        }
      }
      bool IsFinish() const
      {
        return m_moveable.IsValid() ? m_moveable->IsReachDestination() : true;
      }
    private:
      TWeakInterfacePtr<IMoveable> m_moveable;
      TWeakInterfacePtr<ISmithAnimator> m_animator;
  };
  #pragma endregion MoveCommand PImpl
  // end of MoveCommand PImpl

  MoveCommand::MoveCommand(IMoveable* moveable, ISmithAnimator* animator)
    : m_moveImpl(new MoveImpl(moveable, animator))
  {}

  MoveCommand::~MoveCommand()
  {
    m_moveImpl.Reset();
    memset(this, 0, sizeof(*this));
  }

  void MoveCommand::Start()
  {
    m_moveImpl->Start();
  }

  void MoveCommand::Execute(float deltaTime)
  {
    m_moveImpl->Update(deltaTime);
  }

  void MoveCommand::End()
  {
    m_moveImpl->End();
  }

  bool MoveCommand::IsFinish() const
  {
    return m_moveImpl->IsFinish();
  }
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveCommand.h"
#include "SmithPlayerActor.h"
#include "SmithMoveComponent.h"
#include "IMoveable.h"
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
      MoveImpl(IMoveable* moveable)
        : m_moveable(moveable)
      { }
      ~MoveImpl()
      {
        m_moveable.Reset();
      }
    public:
      void Start()
      {

      }
      void Update(float deltaTime)
      {
        FString moveStr{};
        if (m_moveable.IsValid())
        {
          moveStr.Append(m_moveable->_getUObject()->GetName());
          m_moveable->Move();
        }
        else
        {
          moveStr.Append(TEXT("EMPTY OBJECT"));
        }

        moveStr.Append(TEXT(" Move Command"));
        MDebug::Log(moveStr);
      }
      void End()
      {

      }
      bool IsFinish() const
      {
        return true;
      }
    private:
      TWeakInterfacePtr<IMoveable> m_moveable;
  };
  #pragma endregion MoveCommand PImpl
  // end of MoveCommand PImpl

  MoveCommand::MoveCommand(IMoveable* moveable)
    : m_moveImpl(new MoveImpl(moveable))
  {}

  MoveCommand::~MoveCommand()
  {
    m_moveImpl.Reset();
    memset(this, 0, sizeof(this));
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

#include "EnhanceCommand.h"
#include "UObject/WeakInterfacePtr.h"
#include "EnhanceSystem.h"

#include "Debug.h"

namespace UE::Smith::Command
{
  // MoveCommand実装
  // MoveCommand PImpl
  #pragma region EnhanceCommand PImpl
  class EnhanceCommand::EnhanceImpl
  {
    public:
      EnhanceImpl()
      { }
      ~EnhanceImpl()
      {
        m_enhance.Reset();
      }
    public:
      void Start()
      {

      }
      void Update(float deltaTime)
      {
        FString enhanceStr{};
        if (m_enhance.IsValid())
        {
          //moveStr.Append(m_enhance->_getUObject()->GetName());
          m_enhance->Enhance();
        }
        else
        {
          enhanceStr.Append(TEXT("EMPTY OBJECT"));
        }

        enhanceStr.Append(TEXT(" Enhance Command"));
        MDebug::Log(enhanceStr);
      }
      void End()
      {

      }
      bool IsFinish() const
      {
        return true;
      }
    private:
      TWeakInterfacePtr<EnhanceSystem> m_enhance;
  };
  #pragma endregion MoveCommand PImpl
  // end of MoveCommand PImpl

  EnhanceCommand::EnhanceCommand()
    : m_enhanceImpl(new EnhanceImpl())
  {}

  EnhanceCommand::~EnhanceCommand()
  {
    m_enhanceImpl.Reset();
    memset(this, 0, sizeof(this));
  }

  void EnhanceCommand::Start()
  {
    m_enhanceImpl->Start();
  }

  void EnhanceCommand::Execute(float deltaTime)
  {
    m_enhanceImpl->Update(deltaTime);
  }

  void EnhanceCommand::End()
  {
    m_enhanceImpl->End();
  }

  bool EnhanceCommand::IsFinish() const
  {
    return m_enhanceImpl->IsFinish();
  }
}
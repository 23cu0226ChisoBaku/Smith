#include "HealCommand.h"
#include "UObject/WeakInterfacePtr.h"
#include "IHealable.h"

namespace UE::Smith::Command
{
  // HealCommand実装
  // HealCommand PImpl
  #pragma region HealCommand PImpl
  class HealCommand::HealImpl
  {
    public:
      HealImpl(IHealable* healable)
      :m_healable(healable)
      { }
      ~HealImpl()
      {
        m_healable.Reset();
      }
    public:
      void Start()
      {

      }
      void Update(float deltaTime)
      {
        m_healable->Heal();
      }
      void End()
      {

      }
      bool IsFinish() const
      {
        return true;
      }
    private:
    TWeakInterfacePtr<IHealable> m_healable;
  };
  #pragma endregion HealCommand PImpl
  // end of HealCommand PImpl

  HealCommand::HealCommand(IHealable* healable)
  :m_healImpl(new HealImpl(healable))
  { }

  HealCommand::~HealCommand()
  {
    m_healImpl.Reset();
    memset(this, 0, sizeof(*this));
  }

  void HealCommand::Start()
  {
    m_healImpl->Start();
  }

  void HealCommand::Execute(float deltaTime)
  {
    m_healImpl->Update(deltaTime);
  }

  void HealCommand::End()
  { 
    m_healImpl->End();
  }

  bool HealCommand::IsFinish() const
  {
    return m_healImpl->IsFinish();
  }
}
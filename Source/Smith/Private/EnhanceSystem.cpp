#include "EnhanceSystem.h"
#include "SmithBattleSubsystem.h"

#include "Debug.h"

EnhanceSystem::EnhanceSystem()
{
}

void EnhanceSystem::Enhance(IEnhanceable* weapon,FParams item)
{
  FParams enhancePoint = weapon->GetParam();
  enhancePoint.HP += item.HP;
  enhancePoint.ATK += item.ATK;
  enhancePoint.DEF += item.DEF;
  enhancePoint.CRT += item.CRT;

  weapon->SetParam(enhancePoint);
}
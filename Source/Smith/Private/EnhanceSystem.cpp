#include "EnhanceSystem.h"
#include "SmithBattleSubsystem.h"

#include "Debug.h"

EnhanceSystem::EnhanceSystem()
{
}

void EnhanceSystem::Enhance(int32 wepon,int32 item)
{
  MDebug::LogError("Enhance Result " + FString::FromInt(wepon + item));
}
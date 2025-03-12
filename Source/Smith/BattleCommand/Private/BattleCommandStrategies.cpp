
#include "BattleCommandStrategies.h"

namespace UE::Smith::Command
{
  void StartCommand(const AttackCommand& AttackCmd)
  {

  }
  void StartCommand(const IdleCommand& IdleCmd)
  {

  }
  void StartCommand(const MoveCommand& MoveCmd)
  {

  }
  void StartCommand(const TestCommand& TestCmd)
  {
    UE_LOG(LogTemp, Warning, TEXT("Start Test Command"));
  }

  void ExecuteCommand(const AttackCommand& AttackCmd, float DeltaTime)
  {

  }
  void ExecuteCommand(const IdleCommand& IdleCmd, float DeltaTime)
  {

  }
  void ExecuteCommand(const MoveCommand& MoveCmd, float DeltaTime)
  {

  }
  void ExecuteCommand(const TestCommand& TestCmd, float DeltaTime)
  {
    UE_LOG(LogTemp, Warning, TEXT("Executing Test Command. DeltaTime = [%f]"), DeltaTime);
  }
  
  void EndCommand(const AttackCommand& AttackCmd)
  {

  }
  void EndCommand(const IdleCommand& IdleCmd)
  {

  }
  void EndCommand(const MoveCommand& MoveCmd)
  {

  }
  void EndCommand(const TestCommand& TestCmd)
  {
    UE_LOG(LogTemp, Warning, TEXT("End Test Command"));
  }
}
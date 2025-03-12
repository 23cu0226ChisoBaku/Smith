#pragma once

#ifndef BATTLE_COMMAND_STRATEGY
#define BATTLE_COMMAND_STRATEGY

namespace UE::Smith::Command
{
  class TestCommand
  {
  
  };
  class AttackCommand;
  class IdleCommand;
  class MoveCommand;

  class TestCommand;

  void StartCommand(const AttackCommand& AttackCmd);
  void StartCommand(const IdleCommand& IdleCmd);
  void StartCommand(const MoveCommand& MoveCmd);
  void StartCommand(const TestCommand& TestCmd);

  void ExecuteCommand(const AttackCommand& AttackCmd, float DeltaTime);
  void ExecuteCommand(const IdleCommand& IdleCmd, float DeltaTime);
  void ExecuteCommand(const MoveCommand& MoveCmd, float DeltaTime);
  void ExecuteCommand(const TestCommand& TestCmd, float DeltaTime);

  void EndCommand(const AttackCommand& AttackCmd);
  void EndCommand(const IdleCommand& IdleCmd);
  void EndCommand(const MoveCommand& MoveCmd);
  void EndCommand(const TestCommand& TestCmd);

}

#endif
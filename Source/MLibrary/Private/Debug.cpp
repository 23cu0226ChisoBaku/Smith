// Fill out your copyright notice in the Description page of Project Settings.
/*
Module MLibrary

Debug.cpp

Author : MAI ZHICONG

Description : Debug Wrapper class for Unreal Engine 5
Support verion: 5.3.2

Update History: 2024/12/03 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "Debug.h"

namespace UE::MLibrary
{
  float Debug::gLogTime = 15.0f;

  const DebugFormat Debug::LOG_FORMAT = { TEXT("[Log]:"), FColor::White };
  const DebugFormat Debug::LOG_WARNING_FORMAT = { TEXT("[Warning]:"), FColor::Yellow };
  const DebugFormat Debug::LOG_ERROR_FORMAT = { TEXT("[Error]:"), FColor::Red };

  void Debug::Log(const char* msg)
  {
    FString log = LOG_FORMAT.logHeaderString + msg;
    log_impl(Display, log);
  }

  void Debug::log_impl(ELogFatalLevel level, const FString& log)
  {
    if (GEngine != nullptr)
    {
      // TODO 継承でswitchを無くす 
      switch(level)
      {
        case Display:
        {
          GEngine->AddOnScreenDebugMessage(-1, gLogTime, LOG_FORMAT.logColor, log);
        }
        break;

        case Warning:
        {
          GEngine->AddOnScreenDebugMessage(-1, gLogTime, LOG_WARNING_FORMAT.logColor, log);
        } 
        break;

        case Error:
        {
          GEngine->AddOnScreenDebugMessage(-1, gLogTime, LOG_ERROR_FORMAT.logColor, log);
        }

        break;

        default:
        {

        }
      }
    }
  }
}


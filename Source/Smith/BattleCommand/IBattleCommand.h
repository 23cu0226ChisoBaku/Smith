// Fill out your copyright notice in the Description page of Project Settings.
/*

IBattleCommand.h

Author : MAI ZHICONG

Description : バトルコマンドインターフェース

Update History: 2024/12/10 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#pragma once

///
/// @brief バトルコマンドインターフェース
///
class IBattleCommand 
{
  public:

    virtual ~IBattleCommand() = 0 {};

    ///
    /// @brief コマンド執行開始する前に呼び出される
    ///
    virtual void Start() = 0;

    ///
    /// @brief 							コマンド執行
    /// @param deltaTime		実行デルタタイム
    ///
    virtual void Execute(float deltaTime) = 0;

    ///
    /// @brief コマンド執行終了した後呼び出される
    ///
    virtual void End() = 0;

    ///
    /// @brief コマンド実行終了したか
    ///
    virtual bool IsFinish() const = 0;
};

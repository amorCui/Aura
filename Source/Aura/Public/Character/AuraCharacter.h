// Copyright Amor

// 预编译头指令，防止头文件被多次包含
#pragma once

// 包含Unreal Engine的核心头文件
#include "CoreMinimal.h"
// 包含Aura角色基类的头文件
#include "Character/AuraCharacterBase.h"
// 包含自动生成的代码头文件
#include "AuraCharacter.generated.h"

/**
 * Aura游戏中的玩家角色类
 * 这个类继承自AAuraCharacterBase，是玩家控制的具体角色
 * 可以在这里添加玩家特有的功能和属性
 */
UCLASS()  // UCLASS宏，向Unreal反射系统注册该类
class AURA_API AAuraCharacter : public AAuraCharacterBase  // 继承自Aura角色基类
{
    // UE宏，生成反射和序列化所需的代码体
    GENERATED_BODY()

public:
    // 构造函数声明
    // 在这里初始化玩家角色特有的属性和组件
    AAuraCharacter();

    // 当角色被控制器拥有时调用（服务器端）
    // 重写自APawn类的虚函数，在服务器端执行
    // @param NewController: 获得角色控制权的新控制器
    // 使用场景：玩家加入游戏、角色重生、控制权转移
    virtual void PossessedBy(AController* NewController) override;

    // 当PlayerState在客户端复制时调用（客户端）
    // 重写自APawn类的虚函数，在客户端执行
    // 使用场景：客户端第一次接收到PlayerState、PlayerState属性变化
    virtual void OnRep_PlayerState() override;

private:
    // 初始化能力系统的Actor信息
    // 这个函数负责将能力系统组件、PlayerState和角色关联起来
    // 功能：
    // 1. 从PlayerState获取能力系统组件和属性集
    // 2. 初始化AbilityActorInfo，建立Owner-Avatar关系
    // 3. 绑定相关委托和事件
    // 注意：这个函数在服务器和客户端都会被调用
    void InitAbilityActorInfo();
};  

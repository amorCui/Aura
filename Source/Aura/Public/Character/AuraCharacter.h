// Copyright Amor

// 预编译头指令，防止头文件被多次包含
#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * Aura游戏中的玩家角色类
 * 这个类继承自AAuraCharacterBase，是玩家控制的具体角色
 * 可以在这里添加玩家特有的功能和属性
 *
 * 类继承层次说明：
 * AAuraCharacter → AAuraCharacterBase → ACharacter → APawn → AActor
 *
 * 设计模式说明：
 * 1. 使用继承层次结构，将通用功能放在基类(AAuraCharacterBase)中
 * 2. 派生类(AAuraCharacter)专注于玩家特有的功能
 * 3. 这种设计支持未来的扩展，如添加AI控制的敌人角色类
 *
 * 功能特点：
 * 1. 玩家控制的角色，响应玩家输入
 * 2. 拥有玩家特有的能力和属性
 * 3. 与Gameplay Ability System(GAS)集成
 * 4. 支持网络游戏中的复制和同步
 */
UCLASS()  // UCLASS宏，向Unreal反射系统注册该类，使其可用于蓝图、序列化等
class AURA_API AAuraCharacter : public AAuraCharacterBase  // 继承自Aura角色基类
{
    // UE宏，生成反射和序列化所需的代码体
    // 这个宏必须放在类定义的顶部，紧接着类名之后
    GENERATED_BODY()

public:
    /**
     * 构造函数声明
     * 在这里初始化玩家角色特有的属性和组件
     * 构造函数在对象创建时调用，用于设置默认值和初始化状态
     */
    AAuraCharacter();

    /**
     * 当角色被控制器拥有时调用（服务器端）
     * 重写自APawn类的虚函数，在服务器端执行
     *
     * @param NewController: 获得角色控制权的新控制器
     *
     * 调用时机：
     * 1. 玩家加入游戏并生成角色时
     * 2. 角色重生时
     * 3. 控制权转移时（如接管AI控制）
     *
     * 功能说明：
     * 1. 在服务器端初始化角色
     * 2. 设置角色与控制器关联
     * 3. 初始化游戏能力系统(GAS)信息
     * 4. 执行玩家特有的服务器端初始化
     *
     * 网络游戏中的角色控制权流程：
     * 服务器 → PossessedBy(NewController) → 客户端 → OnRep_PlayerState()
     */
    virtual void PossessedBy(AController* NewController) override;

    /**
     * 当PlayerState在客户端复制时调用（客户端）
     * 重写自APawn类的虚函数，在客户端执行
     *
     * 调用时机：
     * 1. 客户端第一次接收到PlayerState（玩家加入游戏时）
     * 2. PlayerState属性发生变化并被复制到客户端时
     * 3. 控制器在客户端被设置时
     *
     * 功能说明：
     * 1. 在客户端初始化角色
     * 2. 设置UI和视觉反馈
     * 3. 初始化客户端的游戏能力系统(GAS)信息
     * 4. 执行玩家特有的客户端初始化
     *
     * 注意：这个函数只在客户端调用，用于处理复制的数据
     */
    virtual void OnRep_PlayerState() override;

private:
    /**
     * 初始化能力系统的Actor信息
     * 这个函数负责将能力系统组件、PlayerState和角色关联起来
     *
     * 功能：
     * 1. 从PlayerState获取能力系统组件(ASC)和属性集
     * 2. 初始化AbilityActorInfo，建立Owner-Avatar关系
     * 3. 绑定相关委托和事件
     * 4. 初始化玩家的输入绑定（如果使用增强输入系统）
     *
     * GAS架构中的Owner-Avatar概念：
     * - Owner：拥有能力系统的对象（通常是PlayerState）
     * - Avatar：执行能力的具体角色（通常是Character）
     * - 这种分离允许角色死亡/重生时保持能力状态
     *
     * 注意：这个函数在服务器和客户端都会被调用
     * - 服务器端：在PossessedBy中调用
     * - 客户端：在OnRep_PlayerState中调用
     * 这确保了两端的GAS系统正确初始化
     */
    void InitAbilityActorInfo();

}; 


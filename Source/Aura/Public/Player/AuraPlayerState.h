// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

// 前向声明能力系统相关类，避免不必要的头文件包含
// UAbilitySystemComponent: 游戏能力系统的核心组件，负责管理角色的所有能力、效果和属性
class UAbilitySystemComponent;

// UAttributeSet: 属性集基类，定义和管理角色的各种属性
class UAttributeSet;

/**
 * Aura玩家状态类
 * 继承自APlayerState并实现IAbilitySystemInterface接口
 *
 * 功能说明：
 * 1. 存储玩家状态信息（如等级、经验、分数等）
 * 2. 作为Gameplay Ability System(GAS)的拥有者，持有能力系统组件和属性集
 * 3. 在多人游戏中，PlayerState会在服务器和所有客户端之间复制
 *
 * GAS架构说明：
 * 在GAS框架中，有三种常见的GAS拥有者模式：
 * 1. 角色(Character)作为拥有者：适用于AI控制或玩家控制的角色
 * 2. 玩家状态(PlayerState)作为拥有者：适用于玩家控制的角色，状态在死亡/重生时保持不变
 * 3. 玩家控制器(PlayerController)作为拥有者：较少使用，适合纯粹客户端能力
 *
 * 选择PlayerState作为拥有者的优势：
 * - 玩家状态在角色死亡和重生时保持不变
 * - 适合存储永久性的玩家进度和属性
 * - 在网络游戏中，PlayerState会自动复制到所有客户端
 */
UCLASS()  // UCLASS宏，向Unreal反射系统注册该类
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
    // UE宏，生成反射和序列化所需的代码体
    GENERATED_BODY()

public:
    /**
     * 构造函数
     * 初始化玩家状态对象
     */
    AAuraPlayerState();

    /** Start IAbilitySystemInterface */
    /**
     * 实现 IAbilitySystemInterface 接口的 GetAbilitySystemComponent 函数
     * 这个函数用于获取角色的能力系统组件，是 GAS 框架的入口点
     *
     * @return 返回指向角色能力系统组件的指针，如果没有则返回 nullptr
     *
     * 功能说明：
     * 1. 这是 IAbilitySystemInterface 接口的纯虚函数，必须实现
     * 2. 允许外部系统通过统一的接口获取角色的能力系统
     * 3. 在多人游戏中，服务器和客户端都会调用这个函数
     * 4. 返回的能力系统组件用于应用能力、效果和查询属性
     */
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    /** End IAbilitySystemInterface */

    /**
     * 获取角色属性集的Getter函数
     *
     * @return 返回指向角色属性集的指针，如果未设置则返回nullptr
     *
     * 功能说明：
     * 1. 这是一个内联的getter函数，提供对AttributeSet的安全访问
     * 2. 返回类型是UAttributeSet*，这是GAS中属性集的基类指针
     * 3. 可以进一步扩展为返回具体类型的属性集（如UAuraAttributeSet*）
     * 4. 属性集存储角色的所有游戏属性（生命值、法力值、攻击力等）
     */
    UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
    /**
     * 能力系统组件指针
     * UPROPERTY()宏使其受到Unreal垃圾回收系统的管理
     *
     * 功能说明：
     * 1. TObjectPtr是UE5推荐的智能指针类型，用于安全地引用UObject对象
     * 2. UAbilitySystemComponent: 游戏能力系统的核心组件，负责管理角色的所有能力、效果和属性
     * 3. 该组件将在此类的构造函数中创建并初始化
     * 4. 在GAS中，ASC(AbilitySystemComponent)是能力的执行者，处理能力的激活、取消和冷却
     */
    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    /**
     * 属性集指针
     * 定义和管理角色的各种属性（如生命值、法力值、攻击力等）
     *
     * 功能说明：
     * 1. UAttributeSet: 属性集，定义和管理角色的各种属性
     * 2. 属性集包含属性的实际值，并处理属性的修改和变化
     * 3. 常见的属性包括：生命值(Health)、法力值(Mana)、力量(Strength)、敏捷(Agility)等
     * 4. 属性通过GameplayEffects进行修改，支持复杂的属性计算（如百分比加成、临时buff等）
     */
    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;

    // 注意：根据GAS最佳实践，通常在构造函数中初始化AbilitySystemComponent，
    // 并在PostInitializeComponents或BeginPlay中创建和设置AttributeSet

}; 
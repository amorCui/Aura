// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

// 前向声明能力系统相关类，避免不必要的头文件包含
// UAbilitySystemComponent: 游戏能力系统的核心组件，负责管理角色的所有能力、效果和属性
class UAbilitySystemComponent;

// UAttributeSet: 属性集基类，定义和管理角色的各种属性
class UAttributeSet;

/**
 * Aura角色基类
 * 这是一个抽象基类，用于游戏中所有角色的基础功能
 * 使用Abstract标记，表示不能被直接实例化，必须被子类化
 *
 * 类继承层次说明：
 * AAuraCharacterBase → ACharacter → APawn → AActor
 *
 * 设计模式说明：
 * 1. 抽象基类模式：提供通用的角色功能，具体角色类继承并实现特定功能
 * 2. 玩家角色和敌人角色都可以从这个基类派生
 * 3. 统一管理Gameplay Ability System(GAS)组件和属性集
 *
 * 核心功能：
 * 1. 集成Gameplay Ability System(GAS)框架
 * 2. 管理角色属性和能力
 * 3. 提供角色基础框架（武器、动画、状态等）
 * 4. 支持网络游戏中的复制和同步
 */
UCLASS(Abstract)  // 标记为抽象类，不能在编辑器中直接创建实例
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
    // UE宏，生成反射和序列化所需的代码体
    GENERATED_BODY()

public:
    /**
     * 构造函数
     * 初始化角色基类的基本属性和组件
     */
    AAuraCharacterBase();

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
     * 重写父类的BeginPlay函数，在角色开始游戏时调用
     * BeginPlay在Actor完全初始化并准备好开始游戏后执行
     *
     * 功能说明：
     * 1. 调用父类的BeginPlay确保基础初始化完成
     * 2. 执行角色特有的初始化逻辑
     * 3. 可以在这里初始化能力、设置初始状态等
     */
    virtual void BeginPlay() override;

    /**
     * 武器组件
     * UPROPERTY宏用于向Unreal引擎暴露属性，使其在编辑器中可编辑并支持垃圾回收
     *
     * 属性宏参数说明：
     * - EditAnywhere: 属性可以在任何地方编辑（实例和蓝图）
     * - Category = "Combat": 在编辑器中分类为"Combat"组，便于组织相关属性
     *
     * TObjectPtr是UE5的智能指针类型，用于安全地引用UObject对象
     * USkeletalMeshComponent: 骨骼网格组件，用于显示武器的3D模型
     *
     * 设计说明：
     * 1. 武器作为独立的组件，便于切换和动画控制
     * 2. 可以在蓝图中设置不同的武器模型
     * 3. 支持武器特效和动画事件
     */
    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<USkeletalMeshComponent> Weapon;

    /**
     * 能力系统组件指针
     * UPROPERTY()宏使其受到Unreal垃圾回收系统的管理
     *
     * 功能说明：
     * 1. TObjectPtr是UE5推荐的智能指针类型，用于安全地引用UObject对象
     * 2. UAbilitySystemComponent: 游戏能力系统的核心组件，负责管理角色的所有能力、效果和属性
     * 3. 该组件将在此类的构造函数中创建并初始化
     * 4. 在GAS中，ASC(AbilitySystemComponent)是能力的执行者，处理能力的激活、取消和冷却
     *
     * 注意：这个组件可能被子类（如玩家角色）设置为从PlayerState获取，而不是自己创建
     * 这是GAS的常见设计模式，特别是对于玩家角色
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
     *
     * 注意：这个属性集可能被子类设置为从PlayerState获取，而不是自己创建
     */
    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;

    // 注意：根据GAS最佳实践，玩家角色通常将ASC和AttributeSet放在PlayerState中
    // 而AI控制的敌人角色可以直接放在Character中
};  
// Copyright Amor
// 预编译头指令，防止头文件被多次包含
#pragma once

// 包含Unreal Engine的核心头文件
#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"

// 包含Unreal Engine的角色基类
#include "GameFramework/Character.h"

// 包含自动生成的代码头文件
#include "AuraCharacterBase.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;


/**
 * Aura角色基类
 * 这是一个抽象基类，用于游戏中所有角色的基础功能
 * 使用Abstract标记，表示不能被直接实例化，必须被子类化
 */ 
UCLASS(Abstract)									  // 标记为抽象类，不能在编辑器中直接创建实例
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY() // UE4/UE5的宏，用于生成必要的反射代码

public:
    // 构造函数
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
     */
    UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
    // 重写父类的BeginPlay函数，在角色开始游戏时调用
    virtual void BeginPlay() override;

    // 武器组件
    // UPROPERTY宏用于向Unreal引擎暴露属性，使其在编辑器中可编辑并支持垃圾回收
    // EditAnywhere: 属性可以在任何地方编辑（实例和蓝图）
    // Category = "Combat": 在编辑器中分类为"Combat"组
    // TObjectPtr是UE5的智能指针类型，用于安全地引用UObject对象
    UPROPERTY(EditAnywhere, Category = "Combat")
    TObjectPtr<USkeletalMeshComponent> Weapon; // 指向武器骨架网格组件的指针

    // Ability System Component (能力系统组件) 和 Attribute Set (属性集) 的声明
    // 这两个是Gameplay Ability System (GAS) 框架的核心组件
    UPROPERTY()
    // TObjectPtr是UE5推荐的智能指针类型，用于安全地引用UObject对象
    // UAbilitySystemComponent: 游戏能力系统的核心组件，负责管理角色的所有能力、效果和属性
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
    UPROPERTY()
    // UAttributeSet: 属性集，定义和管理角色的各种属性（如生命值、法力值、攻击力等）
    TObjectPtr<UAttributeSet> AttributeSet;
};
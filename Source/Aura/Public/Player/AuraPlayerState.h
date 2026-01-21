// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AAuraPlayerState();

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

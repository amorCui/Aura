// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

/**
 * AURA角色属性集类
 * 继承自Unreal Engine的UAttributeSet，用于定义和管理游戏中的角色属性
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    /** 构造函数 */
    UAuraAttributeSet();

    /**
     * 重写父类函数，用于声明需要网络复制的属性
     * @param OutLifetimeProps 输出参数，存储需要网络复制的属性列表
     */
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    //=============================================
    // 基础生命值属性
    //=============================================

    /**
     * 当前生命值属性
     * BlueprintReadOnly: 在蓝图中只读
     * ReplicatedUsing = OnRep_Health: 网络复制时使用指定的回调函数
     * Category = "Vital Attributes": 在编辑器中的分类为"Vital Attributes"
     */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
    FGameplayAttributeData Health;

    /**
     * 最大生命值属性
     * 定义角色能够达到的最大生命值上限
     */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
    FGameplayAttributeData MaxHealth;

    //=============================================
    // 基础法力值属性
    //=============================================

    /**
     * 当前法力值属性
     * 用于施放技能消耗的资源
     */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
    FGameplayAttributeData Mana;

    /**
     * 最大法力值属性
     * 定义角色能够达到的最大法力值上限
     */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
    FGameplayAttributeData MaxMana;

    //=============================================
    // 属性复制回调函数
    // 当属性在服务器端发生变化并在客户端复制时触发
    //=============================================

    /**
     * 生命值变化时的网络复制回调
     * @param OldHealth 变化前的生命值，用于比较和计算差值
     */
    UFUNCTION()
    void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

    /**
     * 最大生命值变化时的网络复制回调
     * @param OldMaxHealth 变化前的最大生命值
     */
    UFUNCTION()
    void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

    /**
     * 法力值变化时的网络复制回调
     * @param OldMana 变化前的法力值
     */
    UFUNCTION()
    void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

    /**
     * 最大法力值变化时的网络复制回调
     * @param OldMaxMana 变化前的最大法力值
     */
    UFUNCTION()
    void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

};

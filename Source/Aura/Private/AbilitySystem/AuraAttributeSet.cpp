// Copyright Amor


#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"



//=============================================
// UAuraAttributeSet 类实现
//=============================================

/**
 * 构造函数
 * 初始化属性集对象，目前没有特殊的初始化逻辑
 */
UAuraAttributeSet::UAuraAttributeSet()
{
    // 构造函数体当前为空，可根据需要在此处添加初始化代码

    InitHealth(50.f);
    InitMaxHealth(100.f);
    InitMana(50.f);
    InitMaxMana(50.f);
}

/**
 * 获取生命周期复制属性
 * 重写此函数以声明哪些属性需要进行网络复制
 * @param OutLifetimeProps 输出参数，用于存储需要网络复制的属性配置列表
 */
void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    // 调用父类实现，确保继承的复制逻辑正常执行
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    /**
     * 声明Health属性需要进行网络复制
     * DOREPLIFETIME_CONDITION_NOTIFY 宏参数说明：
     * 1. UAuraAttributeSet: 当前类名
     * 2. Health: 要复制的属性名
     * 3. COND_None: 复制条件 - 无条件复制（总是复制）
     * 4. REPNOTIFY_Always: 通知策略 - 总是触发OnRep回调（即使值未改变也会调用）
     */
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);

    // 声明MaxHealth属性需要进行网络复制（条件与策略同上）
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

    // 声明Mana属性需要进行网络复制（条件与策略同上）
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

    // 声明MaxMana属性需要进行网络复制（条件与策略同上）
    DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

/**
 * 生命值网络复制回调函数
 * 当Health属性在服务器端更新并在客户端复制完成后调用
 * @param OldHealth 复制前的旧生命值，用于比较和可能的差值计算
 */
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
    /**
     * GAMEPLAYATTRIBUTE_REPNOTIFY 宏用于处理属性复制通知
     * 宏参数说明：
     * 1. UAuraAttributeSet: 当前类名
     * 2. Health: 发生变化的属性名
     * 3. OldHealth: 属性变化前的旧值
     *
     * 此宏内部会自动处理属性的比较和游戏能力系统的通知
     */
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

/**
 * 最大生命值网络复制回调函数
 * 当MaxHealth属性在服务器端更新并在客户端复制完成后调用
 * @param OldMaxHealth 复制前的旧最大生命值
 */
void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
    // 使用宏处理最大生命值属性的复制通知
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

/**
 * 法力值网络复制回调函数
 * 当Mana属性在服务器端更新并在客户端复制完成后调用
 * @param OldMana 复制前的旧法力值
 */
void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
    // 使用宏处理法力值属性的复制通知
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

/**
 * 最大法力值网络复制回调函数
 * 当MaxMana属性在服务器端更新并在客户端复制完成后调用
 * @param OldMaxMana 复制前的旧最大法力值
 */
void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
    // 使用宏处理最大法力值属性的复制通知
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}
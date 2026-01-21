// Copyright Amor


#include "Player/AuraPlayerState.h"
#include <AbilitySystem/AuraAbilitySystemComponent.h>
#include <AbilitySystem/AuraAttributeSet.h>



AAuraPlayerState::AAuraPlayerState()
{
    // 在角色构造函数中创建并初始化Gameplay Ability System (GAS)的核心组件
    // 1. 创建自定义能力系统组件实例
    // CreateDefaultSubobject<>: 在构造函数中创建UObject子对象的Unreal标准方法
    // "AbilitySystemComponent": 组件名称，在编辑器中标识此组件
    // UAuraAbilitySystemComponent: 自定义的能力系统组件类，继承自UAbilitySystemComponent
    AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");

    // 2. 设置能力系统组件的网络复制模式
    // SetIsReplicated(true): 启用组件的网络复制，使其在网络游戏中被同步到所有客户端
    // 注意：更推荐使用SetReplicationMode()方法设置具体的复制模式
    AbilitySystemComponent->SetIsReplicated(true);

    // 3. 创建自定义属性集实例
    // UAuraAttributeSet: 自定义的属性集类，继承自UAttributeSet
    // "AttributeSet": 组件名称，在编辑器中标识此属性集
    AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

    NetUpdateFrequency = 100.f;
    
}

/**
 * AAuraPlayerState::GetAbilitySystemComponent 实现
 *
 * 返回玩家状态的能力系统组件，这是 GAS 框架的标准接口实现
 *
 * @return 指向UAbilitySystemComponent的指针，如果未设置则返回nullptr
 *
 * 功能说明：
 * 1. 实现了IAbilitySystemInterface接口的纯虚函数
 * 2. 允许其他系统通过PlayerState获取能力系统组件
 * 3. 在多人游戏中特别重要，因为玩家状态在服务器和客户端之间复制
 * 4. 相比将能力系统放在角色上，放在PlayerState上更适合网络游戏
 */
UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
    // 直接返回存储在玩家状态中的能力系统组件指针
    return AbilitySystemComponent;
}

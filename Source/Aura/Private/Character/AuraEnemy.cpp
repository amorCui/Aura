// Copyright Amor


// 包含AuraEnemy类的头文件
#include "Character/AuraEnemy.h"

#include <AbilitySystem/AuraAbilitySystemComponent.h>
#include <AbilitySystem/AuraAttributeSet.h>
#include "Aura/Aura.h"




AAuraEnemy::AAuraEnemy()
{
    // 设置网格（Mesh）对可见性碰撞通道（ECC_Visibility）的响应为阻塞（Block）
    // 这使得当执行射线检测（如鼠标光标检测）时，射线会在这个网格处停止并返回命中结果
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

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
}

/**
 * HighlightActor 函数
 * 当敌人需要被高亮显示时调用（如玩家选中目标或鼠标悬停）
 * 实现敌人被高亮时的视觉效果，通常用于表示当前选中目标
 */
void AAuraEnemy::HighlightActor()
{
    GetMesh()->SetRenderCustomDepth(true);
    GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
    Weapon->SetRenderCustomDepth(true);
    Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

/**
 * UnHighlightActor 函数
 * 当敌人不再需要高亮时调用（如玩家取消选择或鼠标移开）
 * 恢复敌人的原始外观，移除高亮效果
 */
void AAuraEnemy::UnHighlightActor()
{
    GetMesh()->SetRenderCustomDepth(false);
    Weapon->SetRenderCustomDepth(false);

}




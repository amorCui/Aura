// Copyright Amor

#include "Character/AuraEnemy.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

#include "Aura/Aura.h"

/**
 * AAuraEnemy 构造函数
 * 初始化敌人角色的属性和组件，特别是碰撞设置和GAS组件
 * 构造函数在对象创建时执行，用于设置默认值和初始化组件
 */
AAuraEnemy::AAuraEnemy()
{
    /**
     * 设置网格（Mesh）对可见性碰撞通道（ECC_Visibility）的响应为阻塞（Block）
     * 这使得当执行射线检测（如鼠标光标检测）时，射线会在这个网格处停止并返回命中结果
     *
     * 碰撞通道说明：
     * - ECC_Visibility: 可见性碰撞通道，通常用于射线检测，判断物体是否可见
     * - ECR_Block: 阻塞响应，射线检测会在这个物体上停止并返回命中信息
     *
     * 功能说明：
     * 1. 使敌人可以被鼠标光标射线检测到
     * 2. 支持玩家通过鼠标悬停或选择敌人
     * 3. 这是实现IEnemyInterface的基础，确保CursorTrace函数能正确检测到敌人
     */
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    /**
     * 创建自定义能力系统组件实例
     * CreateDefaultSubobject<>: 在构造函数中创建UObject子对象的Unreal标准方法
     *
     * 设计说明：
     * - "AbilitySystemComponent": 组件名称，在编辑器中标识此组件
     * - UAuraAbilitySystemComponent: 自定义的能力系统组件类，继承自UAbilitySystemComponent
     *
     * 敌人GAS设计：
     * 1. 敌人角色直接在自身创建GAS组件，而不是从PlayerState获取
     * 2. 这是因为敌人通常不需要跨死亡保持状态，且不是玩家控制的
     * 3. 简化了敌人AI的逻辑，所有能力状态都在角色自身
     */
    AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");

    /**
     * 设置能力系统组件的网络复制属性和复制模式
     *
     * 1. 启用能力系统组件的网络复制功能
     * SetIsReplicated(true): 将这个组件标记为需要网络复制，确保在网络游戏中被同步到所有客户端
     * 这是基础设置，告诉网络系统这个组件需要被复制
     * 敌人通常需要网络复制，以便所有玩家看到相同的状态
     */
    AbilitySystemComponent->SetIsReplicated(true);

    /**
     * 2. 设置能力系统组件的具体复制模式
     * SetReplicationMode(): 指定组件如何在不同客户端之间同步数据
     * EGameplayEffectReplicationMode::Minimal: 最小复制模式，只复制必要的数据，节省带宽
     *
     * 最小复制模式说明：
     * - 对于AI敌人，通常只需要复制重要的状态变化，不需要像玩家那样频繁同步
     * - 节省网络带宽，特别是有大量敌人时
     * - 复制GameplayEffects但不复制GameplayCues（视觉效果客户端本地处理）
     *
     * 与玩家角色的区别：
     * 玩家角色使用Mixed模式，而敌人使用Minimal模式，因为：
     * 1. 玩家需要客户端预测以减少延迟感
     * 2. 敌人的AI逻辑主要在服务器运行，客户端只需要接收结果
     */
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    /**
     * 创建自定义属性集实例
     * CreateDefaultSubobject<>: 同样使用Unreal的标准组件创建方法
     * "AttributeSet": 组件名称，在编辑器中标识此属性集
     * UAuraAttributeSet: 自定义的属性集类，继承自UAttributeSet
     *
     * 敌人属性集特点：
     * 1. 敌人有自己的属性集，独立于玩家
     * 2. 可以在编辑器中配置敌人的基础属性（生命值、攻击力等）
     * 3. 支持不同的敌人类型有不同的属性配置
     */
    AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

/**
 * BeginPlay 函数
 * 当敌人开始游戏时调用，在Actor完全初始化并准备好开始游戏后执行
 *
 * 功能说明：
 * 1. 调用父类的BeginPlay确保基础初始化完成
 * 2. 初始化敌人的能力系统Actor信息
 * 3. 执行敌人特有的游戏逻辑初始化
 */
void AAuraEnemy::BeginPlay()
{
    /**
     * 首先调用父类的BeginPlay，确保基础初始化完成
     * 父类可能初始化了武器、动画、碰撞等重要组件
     */
    Super::BeginPlay();

    /**
     * 初始化能力系统Actor信息
     * InitAbilityActorInfo(): 建立Owner-Avatar关系，初始化GAS系统
     *
     * 参数说明：
     * - this: OwnerActor，拥有能力系统的Actor（敌人自身）
     * - this: AvatarActor，执行能力的具体Actor（敌人自身）
     *
     * 敌人GAS初始化特点：
     * 1. 敌人既是Owner也是Avatar，因为GAS组件直接存储在敌人身上
     * 2. 这与玩家角色不同，玩家的Owner是PlayerState，Avatar是角色
     * 3. 简化了敌人的GAS架构，适合AI控制的非玩家角色
     *
     * 注意：这个函数在服务器和客户端都会被调用，但可能只会在服务器上激活能力
     */
    AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

/**
 * HighlightActor 函数
 * 当敌人需要被高亮显示时调用（如玩家选中目标或鼠标悬停）
 * 实现敌人被高亮时的视觉效果，通常用于表示当前选中目标
 *
 * 实现方式：
 * 使用Custom Depth渲染（也称为轮廓渲染或模板缓冲渲染）来实现高亮效果
 * 这是一种常见的游戏开发技术，用于突出显示特定物体
 *
 * 技术原理：
 * 1. 启用Custom Depth渲染，使物体在后期处理阶段可以被特殊着色
 * 2. 设置Custom Depth Stencil Value，为不同的物体类型分配不同的模板值
 * 3. 在后期材质中，根据模板值为物体添加轮廓或发光效果
 */
void AAuraEnemy::HighlightActor()
{
    /**
     * 启用角色网格的Custom Depth渲染
     * SetRenderCustomDepth(true): 启用自定义深度渲染，允许后期处理效果
     *
     * 注意事项：
     * 1. 需要在项目设置中启用"Custom Depth-Stencil Pass"
     * 2. 这个功能对性能有一定影响，特别是大量敌人同时高亮时
     * 3. 可以优化为只在需要时启用，不需要时禁用
     */
    GetMesh()->SetRenderCustomDepth(true);

    /**
     * 设置角色网格的Custom Depth模板值为红色高亮
     * SetCustomDepthStencilValue(): 分配一个模板值，用于后期材质识别
     * CUSTOM_DEPTH_RED: 通常在Aura.h中定义的常量，如250
     *
     * 模板值分配策略：
     * - 不同的值可以对应不同的高亮颜色（红、蓝、绿等）
     * - 可以在材质蓝图中根据模板值应用不同的颜色
     * - 支持多色高亮，如敌人、盟友、物品等不同颜色
     */
    GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

    /**
     * 启用武器网格的Custom Depth渲染
     * 武器也需要高亮，以保持视觉一致性
     * 如果武器是独立的网格组件，需要单独设置
     */
    Weapon->SetRenderCustomDepth(true);

    /**
     * 设置武器网格的Custom Depth模板值为红色高亮
     * 使用与角色相同的模板值，确保它们使用相同的高亮颜色
     */
    Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

}

/**
 * UnHighlightActor 函数
 * 当敌人不再需要高亮时调用（如玩家取消选择或鼠标移开）
 * 恢复敌人的原始外观，移除高亮效果
 *
 * 功能说明：
 * 1. 禁用Custom Depth渲染，停止轮廓效果
 * 2. 恢复默认材质外观
 * 3. 停止任何高亮相关的特效或声音
 */
void AAuraEnemy::UnHighlightActor()
{
    /**
     * 禁用角色网格的Custom Depth渲染
     * SetRenderCustomDepth(false): 关闭自定义深度渲染
     *
     * 性能优化：
     * 及时禁用Custom Depth可以节省GPU资源，特别是当很多敌人不再被高亮时
     */
    GetMesh()->SetRenderCustomDepth(false);

    /**
     * 禁用武器网格的Custom Depth渲染
     * 确保武器也恢复正常外观
     */
    Weapon->SetRenderCustomDepth(false);
}
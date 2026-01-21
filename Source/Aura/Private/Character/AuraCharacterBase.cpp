// Copyright Amor

#include "Character/AuraCharacterBase.h"

/**
 * AAuraCharacterBase 构造函数
 * 初始化角色和武器组件
 * 构造函数在对象创建时调用，用于设置默认值和初始化组件
 */
AAuraCharacterBase::AAuraCharacterBase()
{
    /**
     * 设置此角色不需要每帧调用Tick()函数
     * 可以关闭此功能以提高性能，如果不需要的话
     *
     * 性能优化说明：
     * - PrimaryActorTick.bCanEverTick = false：完全禁用Actor的Tick函数
     * - 如果角色不需要每帧更新（如只响应事件、不处理持续逻辑），可以禁用以节省CPU时间
     * - 如果后续需要启用Tick，可以设置为true或在子类中重新启用
     *
     * 权衡考虑：
     * 禁用Tick可以提高性能，但意味着角色不能响应每帧的更新
     * 适合以下情况：
     * 1. 角色逻辑主要响应事件（如碰撞、输入、状态变化）
     * 2. 使用Timer或事件驱动代替持续更新
     * 3. 简单的NPC或静态角色
     */
    PrimaryActorTick.bCanEverTick = false;

    /**
     * 创建武器组件
     * CreateDefaultSubobject: 在构造函数中创建子对象的标准方法
     *
     * 方法说明：
     * - "Weapon": 组件名称，用于在编辑器中识别，也是蓝图中的组件名
     * - USkeletalMeshComponent: 骨骼网格体组件类型，用于显示武器的3D模型
     * - 使用模板参数指定组件类型，返回对应的组件指针
     *
     * 设计考虑：
     * 1. 武器作为独立组件，便于更换和动画控制
     * 2. 可以在蓝图中设置不同的武器模型
     * 3. 支持武器特效和碰撞检测
     */
    Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");

    /**
     * 将武器组件附加到角色的骨骼网格体上
     * SetupAttachment: 将子组件附加到父组件的指定插槽
     *
     * 参数说明：
     * - GetMesh(): 获取角色的骨骼网格体组件（角色模型）
     * - FName("WeaponHandSocket"): 骨骼插槽名称，武器将附着在这个插槽上
     *
     * 技术要求：
     * 1. 骨骼插槽必须在角色的骨骼网格体中预先定义
     * 2. 插槽位置通常在角色的手部或背部
     * 3. 插槽方向决定了武器的朝向和位置
     *
     * 注意：如果角色没有"WeaponHandSocket"插槽，武器可能附着在错误的位置或不可见
     */
    Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));

    /**
     * 设置武器组件的碰撞属性
     * SetCollisionEnabled: 配置组件的碰撞检测行为
     *
     * 参数说明：
     * - ECollisionEnabled::NoCollision: 禁用所有碰撞检测
     *
     * 设计考虑：
     * 1. 防止武器与场景中的其他物体发生不必要的碰撞交互
     * 2. 武器的实际碰撞通常由单独的碰撞体或技能系统处理
     * 3. 如果需要武器碰撞（如近战武器），可以在特定时刻启用碰撞
     *
     * 替代方案：
     * 1. 使用QueryOnly：仅用于射线检测，不影响物理模拟
     * 2. 使用PhysicsOnly：仅用于物理模拟，不影响射线检测
     * 3. 使用QueryAndPhysics：同时用于射线检测和物理模拟
     */
    Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/**
 * AAuraCharacterBase::GetAbilitySystemComponent 实现
 * 返回角色基类的能力系统组件，这是 GAS 框架的标准接口实现
 *
 * @return 指向UAbilitySystemComponent的指针，如果未设置则返回nullptr
 *
 * 功能说明：
 * 1. 实现了IAbilitySystemInterface接口的纯虚函数
 * 2. 允许其他系统通过统一接口获取角色的能力系统组件
 * 3. 在多人游戏中，服务器和客户端都会调用这个函数
 * 4. 提供对能力系统组件的安全访问
 *
 * 设计模式说明：
 * 这个基类实现提供了一个基础的ASC访问，但通常：
 * - 玩家角色：ASC实际存储在PlayerState中，角色通过PlayerState访问
 * - AI敌人角色：ASC直接存储在Character中，角色直接访问
 *
 * 注意：这个实现与AAuraPlayerState::GetAbilitySystemComponent不同
 * 那个是从PlayerState获取ASC，这个是从CharacterBase获取ASC
 */
UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
    /**
     * 直接返回存储在角色基类中的能力系统组件指针
     *
     * 实现细节：
     * 1. 这是一个简单的getter函数，返回成员变量AbilitySystemComponent
     * 2. 返回的是基类指针UAbilitySystemComponent*，提供统一的接口
     * 3. 调用者可以通过这个指针访问所有标准ASC功能
     *
     * 安全考虑：
     * 1. 返回的指针可能为nullptr，如果ASC尚未初始化
     * 2. 调用者应该检查返回的指针是否有效
     * 3. 这个函数是const的，不会修改对象状态
     */
    return AbilitySystemComponent;
}

/**
 * BeginPlay 函数
 * 当游戏开始或角色被生成时调用
 * 这里是角色初始化的主要位置
 *
 * 调用时机：
 * 1. 角色被生成到世界中时
 * 2. 游戏开始时（如果角色已在关卡中）
 * 3. 关卡重新加载时
 *
 * 注意事项：
 * 1. BeginPlay在构造函数之后调用
 * 2. 此时所有组件都已初始化完成
 * 3. 适合执行依赖其他Actor或World状态的初始化
 */
void AAuraCharacterBase::BeginPlay()
{
    /**
     * 首先调用父类的BeginPlay方法，确保所有基础初始化完成
     *
     * 父类BeginPlay可能处理的重要功能：
     * 1. 初始化Actor的组件和子对象
     * 2. 注册事件和委托
     * 3. 设置网络复制和同步
     * 4. 初始化动画系统和状态机
     *
     * 调用Super::BeginPlay()是Unreal Engine的最佳实践，
     * 确保继承链中的所有初始化逻辑正确执行
     */
    Super::BeginPlay();

}
// Copyright Amor

// 包含头文件：Aura角色基类的头文件
#include "Character/AuraCharacterBase.h"

/**
 * AAuraCharacterBase 构造函数
 * 初始化角色和武器组件
 */
AAuraCharacterBase::AAuraCharacterBase()
{
    // 设置此角色不需要每帧调用Tick()函数
    // 可以关闭此功能以提高性能，如果不需要的话
    PrimaryActorTick.bCanEverTick = false; // 禁用角色的每帧更新，节省性能

    // 创建武器组件
    // CreateDefaultSubobject: 在构造函数中创建子对象的标准方法
    // "Weapon": 组件名称，用于在编辑器中识别
    Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");

    // 将武器组件附加到角色的骨骼网格体上
    // GetMesh(): 获取角色的骨骼网格体组件
    // FName("WeaponHandSocket"): 骨骼插槽名称，武器将附着在这个插槽上
    // 这个插槽需要在角色的骨骼网格体中预先定义
    Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));

    // 设置武器组件的碰撞属性
    // ECollisionEnabled::NoCollision: 禁用所有碰撞
    // 这可以防止武器与场景中的其他物体发生不必要的碰撞交互
    Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
    // 直接返回存储在玩家状态中的能力系统组件指针
    return AbilitySystemComponent;
}

/**
 * BeginPlay 函数
 * 当游戏开始或角色被生成时调用
 * 这里是角色初始化的主要位置
 */
void AAuraCharacterBase::BeginPlay()
{
    // 首先调用父类的BeginPlay方法，确保所有基础初始化完成
    Super::BeginPlay();
}
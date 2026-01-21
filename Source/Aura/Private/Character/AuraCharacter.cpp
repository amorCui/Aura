// Copyright Amor


// 包含AuraCharacter类的头文件
#include "Character/AuraCharacter.h"

// 包含角色移动组件相关的头文件
// CharacterMovementComponent是处理角色移动的核心组件
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"

/**
 * AAuraCharacter 构造函数
 * 初始化角色属性和移动组件设置
 * 继承自AAuraCharacterBase，这里是角色特定的初始化逻辑
 */
AAuraCharacter::AAuraCharacter()
{
    // 设置角色移动组件的方向与移动方向同步
    // 当角色移动时，会自动旋转面对移动方向
    // 这对于第三人称视角的RPG游戏非常有用，角色可以自然地转向移动方向
    GetCharacterMovement()->bOrientRotationToMovement = true;

    // 设置角色旋转速率，单位是度/秒
    // FRotator(Pitch, Yaw, Roll): 这里设置Yaw轴旋转速率为400度/秒
    // Yaw控制左右旋转，这个值决定了角色转向的平滑度和速度
    GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

    // 将角色移动限制在一个平面内（通常是与地面平行的平面）
    // 这可以防止角色在移动时出现意外的垂直位移
    GetCharacterMovement()->bConstrainToPlane = true;

    // 在开始时将角色对齐到约束平面
    // 确保角色生成时在正确的平面高度上
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    // 禁止角色使用控制器的俯仰（Pitch）旋转
    // 这意味着角色不会跟随控制器的上下视角而前后倾斜
    bUseControllerRotationPitch = false;

    // 禁止角色使用控制器的翻滚（Roll）旋转
    // 这意味着角色不会跟随控制器的侧向倾斜而左右翻滚
    bUseControllerRotationRoll = false;

    // 禁止角色使用控制器的偏航（Yaw）旋转
    // 这意味着角色不会跟随控制器的左右视角而左右转动
    // 角色的旋转将由bOrientRotationToMovement控制，转向移动方向
    bUseControllerRotationYaw = false;

}

/**
 * PossessedBy - 当角色被控制器拥有时调用（服务器端）
 * 这个函数在服务器上调用，当控制器获得对这个角色的控制权时
 *
 * @param NewController 新的控制器，将控制这个角色
 *
 * 使用场景：
 * 1. 玩家加入游戏时
 * 2. 角色重生时
 * 3. AI控制权转移时
 *
 * 注意：这个函数只在服务器端调用
 */
void AAuraCharacter::PossessedBy(AController* NewController)
{
    // 首先调用父类的PossessedBy，确保基本的控制器设置完成
    Super::PossessedBy(NewController);

    // 初始化能力系统Actor信息
    // 在服务器端，PlayerState已经存在，可以安全地初始化
    InitAbilityActorInfo();
}

/**
 * OnRep_PlayerState - 当PlayerState在客户端复制时调用
 * 这个函数在客户端调用，当PlayerState从服务器复制到客户端时
 *
 * 使用场景：
 * 1. 客户端第一次接收到PlayerState时
 * 2. PlayerState属性发生变化并复制时
 *
 * 注意：这个函数只在客户端调用
 */
void AAuraCharacter::OnRep_PlayerState()
{
    // 首先调用父类的OnRep_PlayerState，确保基本的PlayerState处理完成
    Super::OnRep_PlayerState();

    // 初始化能力系统Actor信息
    // 在客户端，现在有了PlayerState，可以安全地初始化
    InitAbilityActorInfo();
}

/**
 * InitAbilityActorInfo - 初始化能力系统的Actor信息
 * 这个函数负责将能力系统组件、PlayerState和角色关联起来
 *
 * 功能：
 * 1. 从PlayerState获取能力系统组件
 * 2. 初始化AbilityActorInfo，建立Owner-Avatar关系
 * 3. 从PlayerState获取属性集
 *
 * 这个函数在服务器和客户端都会被调用，确保两端的能力系统都正确初始化
 */
void AAuraCharacter::InitAbilityActorInfo()
{
    // 获取AuraPlayerState，这是存储能力系统和属性的地方
    // 使用模板函数获取特定类型的PlayerState
    AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();

    // 使用check宏确保PlayerState存在，如果为nullptr则触发断言错误
    // 这是重要的安全检查，因为后续操作都依赖PlayerState
    check(AuraPlayerState);

    // 从PlayerState获取能力系统组件
    // 这样设计可以将能力系统数据持久化在PlayerState中，即使角色死亡重生也能保持
    AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();

    // 初始化AbilityActorInfo，建立三个关键关系：
    // 1. OwnerActor: PlayerState（拥有能力系统组件的Actor）
    // 2. AvatarActor: 角色本身（执行能力的具体Actor）
    // 这建立了PlayerState->Character的连接，允许能力系统知道谁拥有能力以及谁执行能力
    AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

    // 从PlayerState获取属性集
    // 这样属性数据也持久化在PlayerState中，支持跨角色死亡的属性保持
    AttributeSet = AuraPlayerState->GetAttributeSet();

}

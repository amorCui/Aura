// Copyright Amor

#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"

/**
 * AAuraCharacter 构造函数
 * 初始化角色属性和移动组件设置
 * 继承自AAuraCharacterBase，这里是角色特定的初始化逻辑
 * 构造函数在对象创建时执行，用于设置默认值和初始化组件
 */
AAuraCharacter::AAuraCharacter()
{
    /**
     * 设置角色移动组件的方向与移动方向同步
     * bOrientRotationToMovement = true 时，角色会自动旋转以面对移动方向
     * 这对于第三人称视角的RPG游戏非常有用，角色可以自然地转向移动方向
     * 当角色移动时，会自动调整朝向，而不是始终面向固定方向
     */
    GetCharacterMovement()->bOrientRotationToMovement = true;

    /**
     * 设置角色旋转速率，单位是度/秒
     * FRotator(Pitch, Yaw, Roll): 这里设置Yaw轴旋转速率为400度/秒
     * Yaw控制左右旋转，这个值决定了角色转向的平滑度和速度
     * 400度/秒是一个适中的值，使转向既不会太慢也不会太突兀
     */
    GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

    /**
     * 将角色移动限制在一个平面内（通常是与地面平行的平面）
     * bConstrainToPlane = true 可以防止角色在移动时出现意外的垂直位移
     * 这对于固定高度的地面角色很重要，避免角色意外浮空或下沉
     */
    GetCharacterMovement()->bConstrainToPlane = true;

    /**
     * 在开始时将角色对齐到约束平面
     * bSnapToPlaneAtStart = true 确保角色生成时在正确的平面高度上
     * 这可以避免角色生成时卡在地面以下或浮在半空中的问题
     */
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    /**
     * 禁止角色使用控制器的俯仰（Pitch）旋转
     * bUseControllerRotationPitch = false 意味着角色不会跟随控制器的上下视角而前后倾斜
     * 这在第三人称游戏中很常见，角色只水平旋转，摄像机独立控制俯仰
     */
    bUseControllerRotationPitch = false;

    /**
     * 禁止角色使用控制器的翻滚（Roll）旋转
     * bUseControllerRotationRoll = false 意味着角色不会跟随控制器的侧向倾斜而左右翻滚
     * 这可以防止角色出现不自然的倾斜，保持直立状态
     */
    bUseControllerRotationRoll = false;

    /**
     * 禁止角色使用控制器的偏航（Yaw）旋转
     * bUseControllerRotationYaw = false 意味着角色不会跟随控制器的左右视角而左右转动
     * 角色的旋转将由bOrientRotationToMovement控制，转向移动方向
     * 这提供了更自然的移动体验，角色朝向移动方向而不是摄像机方向
     */
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
 * 注意：这个函数只在服务器端调用，用于权威的初始化
 * 在客户端，相应的初始化在OnRep_PlayerState中完成
 */
void AAuraCharacter::PossessedBy(AController* NewController)
{
    /**
     * 首先调用父类的PossessedBy，确保基本的控制器设置完成
     * 父类可能处理了控制器的绑定、输入设置等重要功能
     */
    Super::PossessedBy(NewController);

    /**
     * 初始化能力系统Actor信息
     * 在服务器端，PlayerState已经存在，可以安全地初始化
     *
     * 流程说明：
     * 1. 服务器创建角色
     * 2. 角色被控制器拥有（PossessedBy被调用）
     * 3. PlayerState此时已经创建并分配给角色
     * 4. 可以初始化GAS系统，绑定能力组件和属性集
     */
    InitAbilityActorInfo();
}

/**
 * OnRep_PlayerState - 当PlayerState在客户端复制时调用
 * 这个函数在客户端调用，当PlayerState从服务器复制到客户端时
 *
 * 使用场景：
 * 1. 客户端第一次接收到PlayerState时（玩家加入游戏）
 * 2. PlayerState属性发生变化并复制时（如等级提升、属性变化）
 *
 * 注意：这个函数只在客户端调用，用于处理复制的数据
 * 服务器端的初始化已经在PossessedBy中完成
 */
void AAuraCharacter::OnRep_PlayerState()
{
    /**
     * 首先调用父类的OnRep_PlayerState，确保基本的PlayerState处理完成
     * 父类可能处理了PlayerState的引用设置、UI更新等功能
     */
    Super::OnRep_PlayerState();

    /**
     * 初始化能力系统Actor信息
     * 在客户端，现在有了PlayerState，可以安全地初始化
     *
     * 流程说明：
     * 1. 服务器创建角色并调用PossessedBy
     * 2. 服务器将PlayerState复制到客户端
     * 3. 客户端接收到PlayerState，触发OnRep_PlayerState
     * 4. 客户端初始化GAS系统，确保两端状态一致
     */
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
 *
 * GAS架构说明：
 * - OwnerActor: 拥有能力系统组件的Actor（通常是PlayerState）
 * - AvatarActor: 执行能力的具体Actor（角色本身）
 * - AbilitySystemComponent: 管理能力的组件
 * - AttributeSet: 存储属性的组件
 */
void AAuraCharacter::InitAbilityActorInfo()
{
    /**
     * 获取AuraPlayerState，这是存储能力系统和属性的地方
     * 使用模板函数GetPlayerState<>()获取特定类型的PlayerState
     * 这个函数返回与角色关联的PlayerState，类型为AAuraPlayerState
     * 在服务器端，这是本地创建的对象；在客户端，这是从服务器复制的对象
     */
    AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();

    /**
     * 使用check宏确保PlayerState存在，如果为nullptr则触发断言错误
     * check()宏在开发构建中有效，如果条件为false则崩溃并显示调用栈
     * 这是重要的安全检查，因为后续操作都依赖PlayerState
     * 如果PlayerState为空，说明角色没有正确设置状态，需要修复
     */
    check(AuraPlayerState);

    /**
     * 从PlayerState获取能力系统组件
     * 这样设计可以将能力系统数据持久化在PlayerState中，即使角色死亡重生也能保持
     *
     * 优势：
     * 1. 角色死亡重生时，能力系统状态保持不变
     * 2. 便于实现角色切换系统
     * 3. 简化了网络复制逻辑
     */
    AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();

    /**
     * 初始化AbilityActorInfo，建立三个关键关系：
     * 1. OwnerActor: PlayerState（拥有能力系统组件的Actor）
     * 2. AvatarActor: 角色本身（执行能力的具体Actor）
     * 这建立了PlayerState->Character的连接，允许能力系统知道谁拥有能力以及谁执行能力
     *
     * InitAbilityActorInfo()函数内部会：
     * - 设置AbilitySystemComponent的OwnerActor和AvatarActor
     * - 注册AttributeSet
     * - 初始化与能力相关的委托和事件
     * - 为客户端预测和网络复制做好准备
     */
    AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

    /**
     * 从PlayerState获取属性集
     * 这样属性数据也持久化在PlayerState中，支持跨角色死亡的属性保持
     *
     * AttributeSet包含所有游戏属性的定义和当前值：
     * - 基础属性：生命值、法力值、力量、敏捷等
     * - 衍生属性：攻击力、防御力、移动速度等
     * - 临时属性：buff/debuff效果
     */
    AttributeSet = AuraPlayerState->GetAttributeSet();


}
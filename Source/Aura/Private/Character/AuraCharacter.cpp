// Copyright Amor


// 包含AuraCharacter类的头文件
#include "Character/AuraCharacter.h"

// 包含角色移动组件相关的头文件
// CharacterMovementComponent是处理角色移动的核心组件
#include "GameFramework/CharacterMovementComponent.h"

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

    // 可以在此处添加更多角色特定的初始化逻辑，例如：
    // - 设置移动速度
    // - 配置跳跃能力
    // - 初始化技能系统
    // - 设置碰撞参数
}
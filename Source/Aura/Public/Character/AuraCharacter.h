// Copyright Amor

// 预编译头指令，防止头文件被多次包含
#pragma once

// 包含Unreal Engine的核心头文件
#include "CoreMinimal.h"
// 包含Aura角色基类的头文件
#include "Character/AuraCharacterBase.h"
// 包含自动生成的代码头文件
#include "AuraCharacter.generated.h"

/**
 * Aura游戏中的玩家角色类
 * 这个类继承自AAuraCharacterBase，是玩家控制的具体角色
 * 可以在这里添加玩家特有的功能和属性
 */
UCLASS()  // UCLASS宏，向Unreal反射系统注册该类
class AURA_API AAuraCharacter : public AAuraCharacterBase  // 继承自Aura角色基类
{
    // UE宏，生成反射和序列化所需的代码体
    GENERATED_BODY()

public:
    // 构造函数声明
    // 在这里初始化玩家角色特有的属性和组件
    AAuraCharacter();

    // 可以在此处添加更多公共成员函数和属性，例如：
    // - 玩家特有的输入处理函数
    // - 玩家状态管理函数
    // - 玩家UI交互接口
    // - 多人游戏相关的网络函数

    // 例如：
    // virtual void PossessedBy(AController* NewController) override;
    // virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // void Interact();
    // void UseSkill(int32 SkillID);

};  // 类定义结束

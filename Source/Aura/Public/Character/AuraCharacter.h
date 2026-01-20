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
};  

// Copyright Amor

#pragma once

// 包含Unreal Engine的核心头文件
#include "CoreMinimal.h"
// 包含Aura角色基类的头文件
#include "Character/AuraCharacterBase.h"
// 包含敌人接口的头文件，这个接口定义了敌人的通用行为
#include "Interaction/EnemyInterface.h"
// 包含自动生成的代码头文件
#include "AuraEnemy.generated.h"

/**
 * Aura游戏中的敌人角色类
 * 这个类继承自AAuraCharacterBase，同时实现了IEnemyInterface接口
 * 用于创建游戏中的敌对单位，如怪物、Boss等
 */
UCLASS()  // UCLASS宏，向Unreal反射系统注册该类
class AURA_API AAuraEnemy : public AAuraCharacterBase,  // 继承自Aura角色基类
    public IEnemyInterface      // 实现敌人接口
{
    // UE宏，生成反射和序列化所需的代码体
    GENERATED_BODY()

public:

    /** Start IEnemyInterface */
    // 重写敌人接口的高亮函数 - 当敌人被玩家选中或鼠标悬停时调用
    // 通常用于视觉反馈，如改变轮廓颜色、发光效果等
    virtual void HighlightActor() override;

    // 重写敌人接口的取消高亮函数 - 当敌人不再被选中或鼠标移开时调用
    // 用于恢复敌人的原始外观
    virtual void UnHighlightActor() override;
    /** End IEnemyInterface */

    AAuraEnemy() ;

protected:
    virtual void BeginPlay() override;
};
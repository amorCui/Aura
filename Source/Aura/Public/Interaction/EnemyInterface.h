// Copyright Amor

#pragma once

// 包含Unreal Engine的核心头文件
#include "CoreMinimal.h"
// 包含Unreal Engine接口系统的基类
#include "UObject/Interface.h"
// 包含自动生成的代码头文件
#include "EnemyInterface.generated.h"

// 这是一个反射系统使用的接口类，用于Unreal Engine的反射和蓝图系统
// 不需要修改这个类，它是由UINTERFACE宏自动生成的
// MinimalAPI：这个接口只在当前模块内使用，不需要导出到其他模块
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
    // 自动生成反射和序列化所需的代码体
    GENERATED_BODY()  
};

/**
 * 敌人接口类
 * 定义所有敌人角色应该实现的基本功能
 * 这是游戏设计中的抽象层，允许不同类型的敌人都能被统一处理
 */
class AURA_API IEnemyInterface  
{
    // 自动生成反射和序列化所需的代码体
    GENERATED_BODY()  


public:
    // 高亮敌人角色的纯虚函数
    // = 0 表示这是纯虚函数，必须在继承的子类中实现
    // 当敌人被玩家选中或鼠标悬停时调用此函数
    virtual void HighlightActor() = 0;

    // 取消高亮敌人角色的纯虚函数
    // = 0 表示这是纯虚函数，必须在继承的子类中实现
    // 当敌人不再被选中或鼠标移开时调用此函数
    virtual void UnHighlightActor() = 0;

};
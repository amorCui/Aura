// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * Aura游戏中的敌人角色类
 * 这个类继承自AAuraCharacterBase，同时实现了IEnemyInterface接口
 * 用于创建游戏中的敌对单位，如怪物、Boss等
 *
 * 类继承层次说明：
 * AAuraEnemy → AAuraCharacterBase → ACharacter → APawn → AActor
 *
 * 接口实现说明：
 * 1. 实现了IEnemyInterface接口，提供敌人特有的交互功能
 * 2. 继承自AAuraCharacterBase，获得基础的GAS功能和角色属性
 *
 * 设计模式说明：
 * 1. 使用多重继承实现接口，保持代码的模块化和可扩展性
 * 2. 将敌人特有的功能放在接口实现中，与基础角色功能分离
 * 3. 支持不同类型的敌人（普通怪物、精英、Boss）共享相同的接口
 *
 * 功能特点：
 * 1. 实现敌人高亮和取消高亮功能，用于玩家交互
 * 2. 继承GAS系统，支持敌人特有的能力和属性
 * 3. 可作为AI控制的目标，支持战斗和AI行为
 */
UCLASS()  // UCLASS宏，向Unreal反射系统注册该类，使其可用于蓝图、序列化等
class AURA_API AAuraEnemy : public AAuraCharacterBase,  // 继承自Aura角色基类
    public IEnemyInterface      // 实现敌人接口
{
    // UE宏，生成反射和序列化所需的代码体
    // 这个宏必须放在类定义的顶部，紧接着类名之后
    GENERATED_BODY()

public:
    /**
     * Start IEnemyInterface接口实现部分
     */

     /**
      * 重写敌人接口的高亮函数 - 当敌人被玩家选中或鼠标悬停时调用
      * 通常用于视觉反馈，如改变轮廓颜色、发光效果等
      *
      * 功能说明：
      * 1. 实现IEnemyInterface接口的纯虚函数
      * 2. 当敌人被玩家选择或鼠标悬停时触发
      * 3. 通常用于显示敌人轮廓、改变材质或播放特效
      *
      * 实现建议：
      * 1. 可以修改材质参数，增加发光或轮廓效果
      * 2. 可以播放高亮动画或粒子效果
      * 3. 可以在敌人头顶显示UI指示器
      * 4. 需要考虑网络游戏中的同步问题
      */
    virtual void HighlightActor() override;

    /**
     * 重写敌人接口的取消高亮函数 - 当敌人不再被选中或鼠标移开时调用
     * 用于恢复敌人的原始外观
     *
     * 功能说明：
     * 1. 实现IEnemyInterface接口的纯虚函数
     * 2. 当敌人失去焦点或被取消选择时触发
     * 3. 撤销所有高亮效果，恢复默认外观
     *
     * 实现建议：
     * 1. 重置材质参数到默认值
     * 2. 停止高亮相关的动画和特效
     * 3. 隐藏UI指示器
     */
    virtual void UnHighlightActor() override;

    /**
     * End IEnemyInterface接口实现部分
     */

     /**
      * 构造函数声明
      * 在这里初始化敌人角色特有的属性和组件
      * 构造函数在对象创建时调用，用于设置默认值和初始化状态
      */
    AAuraEnemy();

protected:
    /**
     * 重写父类的BeginPlay函数，在角色开始游戏时调用
     * BeginPlay在Actor完全初始化并准备好开始游戏后执行
     *
     * 功能说明：
     * 1. 调用父类的BeginPlay确保基础初始化完成
     * 2. 执行敌人特有的初始化逻辑
     * 3. 可以在这里初始化AI行为、设置初始状态等
     *
     * 敌人特有的初始化可能包括：
     * 1. 设置AI控制器和默认行为
     * 2. 初始化敌人的属性和能力
     * 3. 设置巡逻路线或初始位置
     * 4. 初始化视觉和声音组件
     */
    virtual void BeginPlay() override;


};

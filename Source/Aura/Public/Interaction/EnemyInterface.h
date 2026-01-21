// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "EnemyInterface.generated.h"

/**
 * 反射接口类（UEnemyInterface）
 * 这是一个反射系统使用的接口类，用于Unreal Engine的反射和蓝图系统
 * 注意：这个类是UInterface的子类，主要用于蓝图系统，实际功能在下面的IEnemyInterface中定义
 *
 * UINTERFACE宏参数说明：
 * - MinimalAPI：这个接口只在当前模块内使用，不需要导出到其他模块
 *   这可以优化编译速度，减少符号导出
 *
 * 设计说明：
 * 在UE的接口系统中，有两个相关类：
 * 1. UEnemyInterface（这个类）：用于反射和蓝图系统，由UINTERFACE宏自动生成
 * 2. IEnemyInterface（下面定义的类）：实际的功能接口，包含纯虚函数定义
 */
UINTERFACE(MinimalAPI, BlueprintType)  // 添加BlueprintType以支持在蓝图中使用该接口
class UEnemyInterface : public UInterface
{
    // 自动生成反射和序列化所需的代码体
    // 这个宏必须放在UINTERFACE类定义的顶部
    GENERATED_BODY()
};

/**
 * 敌人接口类（IEnemyInterface）
 * 定义所有敌人角色应该实现的基本功能
 * 这是游戏设计中的抽象层，允许不同类型的敌人都能被统一处理
 *
 * 接口设计模式说明：
 * 1. 接口定义了"做什么"（行为规范），而不关心"怎么做"（具体实现）
 * 2. 任何实现了这个接口的类都必须提供HighlightActor和UnHighlightActor的具体实现
 * 3. 这使得系统可以通过IEnemyInterface*指针统一处理所有敌人，而不需要知道具体类型
 *
 * 使用场景：
 * 1. 玩家选择/瞄准敌人时的高亮效果
 * 2. 敌人被攻击时的反馈
 * 3. UI系统显示敌人信息
 * 4. AI系统识别敌人状态
 */
class AURA_API IEnemyInterface
{
    // 自动生成反射和序列化所需的代码体
    // 这个宏必须放在IEnemyInterface类定义的顶部
    // 注意：即使接口类没有继承UObject，也需要这个宏来支持UE的反射系统
    GENERATED_BODY()

public:
    /**
     * 高亮敌人角色的纯虚函数
     * = 0 表示这是纯虚函数，必须在继承的子类中实现
     *
     * 功能说明：
     * 当敌人被玩家选中或鼠标悬停时调用此函数
     * 通常用于视觉反馈，例如：
     * - 显示敌人轮廓高亮
     * - 增加材质发光效果
     * - 播放高亮动画
     * - 显示敌人信息UI
     *
     * 调用时机：
     * 1. 玩家将鼠标悬停在敌人身上（CursorTrace函数检测到）
     * 2. 玩家通过技能或UI选择敌人
     * 3. 游戏剧情需要强调某个敌人时
     *
     * @注意：这个函数通常应该在客户端调用，因为视觉效果只在本地需要
     */
    virtual void HighlightActor() = 0;

    /**
     * 取消高亮敌人角色的纯虚函数
     * = 0 表示这是纯虚函数，必须在继承的子类中实现
     *
     * 功能说明：
     * 当敌人不再被选中或鼠标移开时调用此函数
     * 用于撤销高亮效果，恢复敌人正常外观
     *
     * 调用时机：
     * 1. 鼠标从敌人身上移开
     * 2. 玩家取消选择敌人
     * 3. 敌人死亡或被消灭
     *
     * 实现注意事项：
     * - 应该完全撤销HighlightActor所做的所有修改
     * - 需要确保多次调用不会产生错误
     * - 应该处理敌人被销毁时的边缘情况
     */
    virtual void UnHighlightActor() = 0;

};  

// 注意：实现这个接口的类需要：
// 1. 在类声明中添加：public IEnemyInterface
// 2. 实现HighlightActor()和UnHighlightActor()函数
// 3. 在UCLASS宏中添加：Blueprintable（如果需要在蓝图中实现）
// 4. 在UPROPERTY中可能需要添加Replicated标记，如果需要在网络游戏中同步高亮状态
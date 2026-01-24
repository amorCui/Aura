// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayEffect.h"
#include "OverlayWidgetController.generated.h"

/**
 * 委托声明：属性变化委托
 * 这些委托用于在游戏属性发生变化时通知UI进行更新
 * 使用DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam宏声明带有一个参数的动态多播委托
 *
 * 动态多播委托特点：
 * 1. 动态委托（Dynamic）：支持序列化和蓝图系统，可以在蓝图中绑定和使用
 * 2. 多播委托（Multicast）：可以绑定多个回调函数，同时通知多个监听者
 * 3. 单参数（OneParam）：带有一个参数，传递属性变化后的新值
 *
 * 设计模式：观察者模式（Observer Pattern）
 * 当游戏属性变化时，控制器通知所有绑定到这些委托的UI元素进行更新
 */

 /**
  * 生命值变化委托
  * 当角色的生命值属性发生变化时触发
  *
  * 参数说明：
  * - float, NewHealth: 变化后的新生命值
  *
  * 使用场景：
  * 1. 更新血条UI的填充量
  * 2. 更新生命值数字显示
  * 3. 播放生命值变化的动画或效果
  */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);

/**
 * 最大生命值变化委托
 * 当角色的最大生命值属性发生变化时触发
 *
 * 参数说明：
 * - float, NewMaxHealth: 变化后的新最大生命值
 *
 * 使用场景：
 * 1. 调整血条UI的总长度
 * 2. 更新生命值上限的文本显示
 * 3. 重新计算生命值百分比
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);

/**
 * 法力值变化委托
 * 当角色的法力值属性发生变化时触发
 *
 * 参数说明：
 * - float, NewMana: 变化后的新法力值
 *
 * 使用场景：
 * 1. 更新法力条UI的填充量
 * 2. 更新法力值数字显示
 * 3. 播放法力值变化的动画或效果
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);

/**
 * 最大法力值变化委托
 * 当角色的最大法力值属性发生变化时触发
 *
 * 参数说明：
 * - float, NewMaxMana: 变化后的新最大法力值
 *
 * 使用场景：
 * 1. 调整法力条UI的总长度
 * 2. 更新法力值上限的文本显示
 * 3. 重新计算法力值百分比
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

/**
 * 叠加界面控制器类
 * 继承自UAuraWidgetController，专门管理游戏主叠加界面（Overlay）的逻辑
 *
 * 类特性：
 * - BlueprintType: 可以在蓝图中作为类型使用（如变量类型、函数参数等）
 * - Blueprintable: 可以在蓝图中创建此类的子类或实例
 *
 * 功能说明：
 * 1. 作为游戏主界面的控制器，管理血条、蓝条、技能栏等UI元素
 * 2. 监听游戏属性变化并通知UI更新
 * 3. 处理主界面的输入和交互逻辑
 * 4. 提供蓝图可访问的委托，使设计师可以在蓝图中绑定UI事件
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
    GENERATED_BODY()

public:
    /**
     * 重写广播初始值函数
     * 在界面初始化时调用，向UI发送当前属性的初始值
     *
     * 重写说明：
     * 1. 必须使用override关键字，确保正确重写基类虚函数
     * 2. 使用virtual关键字保持虚函数特性，支持进一步继承和重写
     * 3. 与基类的BroadCastInitialValues保持相同的函数签名
     *
     * 实现内容：
     * 1. 从属性集获取当前属性值
     * 2. 通过委托广播给所有绑定的UI元素
     * 3. 确保UI在初始化时显示正确的数值
     */
    virtual void BroadCastInitialValues() override;


    virtual void BindCallbacksToDependencies() override;

    /**
     * 生命值变化委托（蓝图可分配）
     * UPROPERTY宏参数：
     * - BlueprintAssignable: 蓝图可以分配（绑定）事件到这个委托
     * - Category = "GAS|Attributes": 在蓝图中归入"GAS|Attributes"分类，便于组织
     *
     * 委托功能：
     * 1. 当生命值属性变化时自动触发
     * 2. 可以在蓝图中绑定到UI更新函数
     * 3. 支持多个监听者同时接收通知
     */
    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnHealthChangedSignature OnHealthChanged;

    /**
     * 最大生命值变化委托（蓝图可分配）
     * 当最大生命值属性变化时触发，通知UI更新
     */
    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnMaxHealthChangedSignature OnMaxHealthChanged;

    /**
     * 法力值变化委托（蓝图可分配）
     * 当法力值属性变化时触发，通知UI更新
     */
    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnManaChangedSignature OnManaChanged;

    /**
     * 最大法力值变化委托（蓝图可分配）
     * 当最大法力值属性变化时触发，通知UI更新
     */
    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnMaxManaChangedSignature OnMaxManaChanged;


    /**
     * 属性变化回调函数声明（保护成员）
     *
     * 这些函数作为Gameplay Ability System(GAS)属性变化事件的处理函数，
     * 当游戏中的属性值发生变化时，这些函数会被自动调用。
     *
     * 访问级别：protected
     * - 这些函数仅供类内部和派生类使用，不对外部公开
     * - 防止外部代码直接调用这些回调函数，确保事件处理的封装性
     *
     * 函数特性：const成员函数
     * - 这些函数不会修改类对象的状态，只是处理事件并广播给UI
     * - 符合const正确性，可以在const对象上安全调用
     * - 强调这些函数是"只读"的，不产生副作用
     *
     * 设计模式：事件处理器模式
     * 这些函数作为事件处理器，将GAS系统的属性变化事件转换为UI可以理解的事件
     *
     * 参数说明：
     * const FOnAttributeChangeData& Data: 属性变化数据结构体，包含：
     *   - OldValue: 变化前的属性值
     *   - NewValue: 变化后的属性值
     *   - GEModData: 导致变化的GameplayEffect修改数据（可选，可能为空）
     *
     * 调用机制：
     * 1. GAS检测到属性变化
     * 2. 触发对应的属性变化委托
     * 3. 委托调用已绑定的回调函数（这些函数）
     * 4. 回调函数处理数据并广播给UI
     *
     * 注意：这些函数通常在BindCallbacksToDependencies函数中绑定到GAS的委托上
     */
protected:
    /**
     * 生命值变化回调函数
     * 当游戏中的生命值属性发生变化时调用
     *
     * 功能：
     * 1. 接收新的生命值
     * 2. 通过OnHealthChanged委托广播给所有监听的UI元素
     *
     * 使用场景：
     * 1. 角色受到伤害或治疗时
     * 2. 生命值上限变化导致当前生命值百分比变化时
     * 3. 角色重生或恢复生命值时
     *
     * UI更新示例：
     * - 更新血条填充量
     * - 更新生命值数字显示
     * - 播放生命值变化动画
     */
    void HealthChanged(const FOnAttributeChangeData& Data) const;

    /**
     * 最大生命值变化回调函数
     * 当最大生命值属性发生变化时调用
     *
     * 功能：
     * 1. 接收新的最大生命值
     * 2. 通过OnMaxHealthChanged委托广播给UI
     *
     * 使用场景：
     * 1. 角色升级时生命值上限增加
     * 2. 装备或技能改变生命值上限时
     * 3. Buff/Debuff临时改变生命值上限时
     *
     * UI更新示例：
     * - 调整血条总长度
     * - 更新最大生命值文本
     * - 重新计算生命值百分比
     */
    void MaxHealthChanged(const FOnAttributeChangeData& Data) const;

    /**
     * 法力值变化回调函数
     * 当法力值属性发生变化时调用
     *
     * 功能：
     * 1. 接收新的法力值
     * 2. 通过OnManaChanged委托广播给UI
     *
     * 使用场景：
     * 1. 施放技能消耗法力时
     * 2. 法力值自然恢复时
     * 3. 使用药水或技能恢复法力时
     *
     * UI更新示例：
     * - 更新法力条填充量
     * - 更新法力值数字显示
     * - 播放法力值变化动画
     */
    void ManaChanged(const FOnAttributeChangeData& Data) const;

    /**
     * 最大法力值变化回调函数
     * 当最大法力值属性发生变化时调用
     *
     * 功能：
     * 1. 接收新的最大法力值
     * 2. 通过OnMaxManaChanged委托广播给UI
     *
     * 使用场景：
     * 1. 角色升级时法力值上限增加
     * 2. 装备或技能改变法力值上限时
     * 3. Buff/Debuff临时改变法力值上限时
     *
     * UI更新示例：
     * - 调整法力条总长度
     * - 更新最大法力值文本
     * - 重新计算法力值百分比
     */
    void MaxManaChanged(const FOnAttributeChangeData& Data) const;

    /**
     * 派生类可以添加更多属性变化回调函数：
     *
     * 示例：
     * void StaminaChanged(const FOnAttributeChangeData& Data) const;
     * void ExperienceChanged(const FOnAttributeChangeData& Data) const;
     * void LevelChanged(const FOnAttributeChangeData& Data) const;
     *
     * 注意：添加新属性回调需要：
     * 1. 在头文件中声明函数
     * 2. 在源文件中实现函数
     * 3. 在BindCallbacksToDependencies中绑定到GAS委托
     * 4. 添加对应的委托声明（如果需要）
     */

     /**
      * 这些函数应该只被以下方式调用：
      *
      * 1. 由GAS系统自动调用（通过委托绑定）
      * 2. 在单元测试中直接调用（用于测试）
      *
      * 不应该被以下方式调用：
      * 1. 外部代码直接调用
      * 2. 在同一帧中多次调用（除非属性确实多次变化）
      *
      * 安全注意事项：
      * 1. 这些函数应该快速返回，避免复杂的计算
      * 2. 广播委托可能会触发复杂的UI更新，要注意性能
      * 3. 确保在析构时解除委托绑定，避免悬空回调
      */

      /**
       * 网络游戏中的注意事项：
       *
       * 1. 这些回调可能在服务器和客户端都被调用
       * 2. 对于本地控制的角色，两端都会触发
       * 3. 对于其他客户端角色，只有客户端通过复制值触发
       * 4. 需要注意网络延迟和预测对UI更新的影响
       *
       * 典型的网络处理：
       * 1. 服务器：权威计算，触发回调但不一定更新UI
       * 2. 客户端：接收复制值，触发回调并更新UI
       * 3. 客户端预测：本地先更新UI，服务器确认后再修正
       */

       /**
        * 错误处理建议：
        *
        * 1. 可以在函数开始时检查关键指针的有效性
        * 2. 可以添加调试日志，帮助追踪属性变化
        * 3. 可以考虑使用断言确保在开发阶段发现问题
        *
        * 示例：
        * void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
        * {
        *     check(OnHealthChanged.IsBound()); // 确保委托有绑定
        *     OnHealthChanged.Broadcast(Data.NewValue);
        * }
        */

        /**
         * 性能优化建议：
         *
         * 1. 这些函数可能会被频繁调用（如持续伤害/治疗）
         * 2. 可以考虑批量处理或延迟更新
         * 3. 可以添加变化阈值，只有变化足够大时才更新UI
         *
         * 示例优化：
         * void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
         * {
         *     // 只有生命值变化超过1%时才更新UI
         *     if (FMath::Abs(Data.NewValue - Data.OldValue) / MaxHealth > 0.01f)
         *     {
         *         OnHealthChanged.Broadcast(Data.NewValue);
         *     }
         * }
         */
    
}; 

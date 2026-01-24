// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
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

}; 

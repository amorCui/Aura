// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 界面控制器参数结构体
 * 用于传递初始化界面控制器所需的所有必要参数
 *
 * 设计目的：
 * 1. 封装所有相关的游戏系统组件，便于一次传递
 * 2. 提供类型安全的数据结构，避免参数顺序错误
 * 3. 支持蓝图系统，可以在蓝图中创建和编辑
 *
 * 使用场景：
 * 1. 初始化HUD和界面控制器
 * 2. 切换不同的游戏状态（如暂停、战斗、对话）
 * 3. 重新绑定UI到不同的角色或游戏对象
 */
USTRUCT(BlueprintType)  // 标记为蓝图类型，可以在蓝图中使用
struct FWidgetControllerParams
{
    GENERATED_BODY()  // UE宏，生成反射和序列化所需的代码体

        /**
         * 默认构造函数
         * 初始化所有指针为nullptr
         * UE的反射系统要求结构体有默认构造函数
         */
        FWidgetControllerParams() {}

    /**
     * 带参数的构造函数
     * 方便在C++代码中快速创建参数结构体
     *
     * @param PC 玩家控制器，处理输入和玩家视角
     * @param PS 玩家状态，存储玩家数据和进度
     * @param ASC 能力系统组件，管理游戏能力和效果
     * @param AS 属性集，存储角色的游戏属性
     */
    FWidgetControllerParams(APlayerController* PC, APlayerState* PS,
        UAbilitySystemComponent* ASC, UAttributeSet* AS)
        : PlayerController(PC)
        , PlayerState(PS)
        , AbilitySystemComponent(ASC)
        , AttributeSet(AS)
    {
        // 构造函数体，参数已通过初始化列表设置
    }

    /**
     * 玩家控制器指针
     * UPROPERTY宏使其受到Unreal垃圾回收系统的管理
     *
     * 属性宏参数：
     * - EditAnywhere: 可以在编辑器中任何地方编辑
     * - BlueprintReadWrite: 蓝图可以读写这个属性
     *
     * 功能说明：
     * 1. 控制玩家角色和摄像机
     * 2. 处理玩家输入事件
     * 3. 管理游戏状态和UI模式
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<APlayerController> PlayerController = nullptr;

    /**
     * 玩家状态指针
     * 存储玩家特定的数据和状态
     *
     * 功能说明：
     * 1. 存储玩家等级、经验、分数等数据
     * 2. 在多人游戏中会在服务器和客户端之间复制
     * 3. 通常作为Gameplay Ability System的拥有者
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<APlayerState> PlayerState = nullptr;

    /**
     * 能力系统组件指针
     * Gameplay Ability System(GAS)的核心组件
     *
     * 功能说明：
     * 1. 管理所有游戏能力的激活、取消和冷却
     * 2. 应用GameplayEffects修改属性
     * 3. 监听属性变化并触发事件
     * 4. 处理能力输入和交互
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

    /**
     * 属性集指针
     * 存储和定义角色的游戏属性
     *
     * 功能说明：
     * 1. 定义角色的属性（生命值、法力值、力量等）
     * 2. 处理属性计算和修改器
     * 3. 提供属性访问和修改的接口
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAttributeSet> AttributeSet = nullptr;

};  

/**
 * Aura界面控制器基类
 * 继承自UObject，作为所有界面控制器的基类
 *
 * 设计模式：MVC模式中的Controller层
 *
 * 职责：
 * 1. 连接游戏数据（Model）和用户界面（View）
 * 2. 监听游戏状态变化并更新UI
 * 3. 处理UI事件并调用相应的游戏逻辑
 * 4. 管理数据绑定和事件订阅
 *
 * 优势：
 * 1. 分离游戏逻辑和UI显示
 * 2. 提高代码的可测试性和可维护性
 * 3. 支持不同的UI实现（如不同平台、不同语言）
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
    GENERATED_BODY()  // UE宏，生成反射和序列化所需的代码体

public:
    /**
     * 设置界面控制器参数（蓝图可调用）
     * 这是初始化界面控制器的关键函数
     *
     * UFUNCTION宏参数：
     * - BlueprintCallable: 蓝图可以调用这个函数
     * - Category = "Widget Controller": 在蓝图中归入"Widget Controller"分类
     *
     * @param WCParams 界面控制器参数结构体，包含所有必要的游戏系统引用
     *
     * 功能说明：
     * 1. 接收并存储游戏系统的引用
     * 2. 为后续的数据绑定和事件监听做准备
     * 3. 可以在运行时重新绑定到不同的游戏对象
     */
    UFUNCTION(BlueprintCallable, Category = "Widget Controller")
    void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

    /**
     * 广播初始值虚函数声明
     *
     * 功能概述：
     * 这是一个纯虚函数（或可重写的虚函数），用于向用户界面广播游戏属性的初始值。
     * 在界面控制器初始化时调用，确保UI在显示时具有正确的初始状态。
     *
     * 设计模式：
     * 这是MVC（Model-View-Controller）模式中的关键组件，作为控制器层的一部分，
     * 负责在数据和视图之间建立初始连接。
     *
     * 函数特性：
     * 1. virtual: 虚函数，支持多态，允许派生类提供不同的实现
     * 2. 通常声明为public或protected，供外部或派生类调用
     * 3. 可能被声明为纯虚函数（= 0）强制派生类实现
     *
     * 实现要求（如果派生）：
     * 1. 从AttributeSet获取当前属性值
     * 2. 通过委托广播这些值给所有绑定的UI元素
     * 3. 确保UI显示与游戏状态同步
     *
     * 调用时机：
     * 1. 在界面控制器完全初始化后
     * 2. 在UI元素已绑定到控制器委托后
     * 3. 在角色属性初始化完成后
     * 4. 在游戏状态重置或重新开始时
     *
     * 典型使用场景：
     * 1. 玩家进入游戏时初始化UI
     * 2. 角色重生或创建新角色时
     * 3. 切换不同角色的控制时
     * 4. 游戏状态重置时
     *
     * 网络游戏注意事项：
     * 1. 在客户端，需要在接收到复制的PlayerState和AttributeSet后调用
     * 2. 在服务器，通常不需要UI更新，但可能用于调试或管理员界面
     * 3. 需要处理客户端预测和服务器权威值的同步
     *
     * 异常处理：
     * 1. 应检查AttributeSet是否为有效指针
     * 2. 应检查委托是否有绑定，避免空广播
     * 3. 应考虑属性值可能不在有效范围内
     *
     * 性能考虑：
     * 1. 只应在初始化时调用一次，避免每帧调用
     * 2. 广播操作可能涉及多个UI元素更新，需确保效率
     * 3. 可以考虑延迟或分批广播优化
     */
    virtual void BroadCastInitialValues();

    virtual void BindCallbacksToDependencies();

protected:
    /**
     * 玩家控制器引用（蓝图只读）
     * 用于UI输入处理和游戏状态控制
     *
     * 属性宏参数：
     * - BlueprintReadOnly: 蓝图可以读取但不能修改
     * - Category = "Widget Controller": 在蓝图中归入"Widget Controller"分类
     *
     * 保护级别说明：子类可以访问这些引用，但外部代码需要通过接口访问
     */
    UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
    TObjectPtr<APlayerController> PlayerController;

    /**
     * 玩家状态引用（蓝图只读）
     * 存储玩家特定数据和游戏进度
     */
    UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
    TObjectPtr<APlayerState> PlayerState;

    /**
     * 能力系统组件引用（蓝图只读）
     * Gameplay Ability System的核心，管理游戏能力和效果
     */
    UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    /**
     * 属性集引用（蓝图只读）
     * 存储角色的游戏属性和状态
     */
    UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
    TObjectPtr<UAttributeSet> AttributeSet;

}; 

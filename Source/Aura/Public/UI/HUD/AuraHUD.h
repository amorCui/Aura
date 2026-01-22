// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"


class UAuraUserWidget;


/**
 * Aura游戏平视显示器（HUD）类
 *
 * 功能概述：
 * 1. 管理游戏用户界面（UI）的显示和隐藏
 * 2. 控制叠加界面（Overlay Widget）的创建和生命周期
 * 3. 作为游戏UI系统的核心控制器
 *
 * 设计模式：
 * 使用主从模式，HUD作为主控制器管理所有UI控件
 */

 /**
  * UCLASS宏，向Unreal反射系统注册该类
  *
  * 类继承层次：
  * AAuraHUD → AHUD → AActor
  *
  * AHUD是Unreal Engine中用于管理游戏界面的基类，提供：
  * 1. 画布绘制功能
  * 2. UI控件管理
  * 3. 屏幕空间坐标转换
  */
UCLASS()
class AURA_API AAuraHUD : public AHUD  // 继承自Unreal Engine的HUD基类
{
    // UE宏，生成反射和序列化所需的代码体
    // 必须放在类定义的顶部，紧接着类名之后
    GENERATED_BODY()

public:
    /**
     * 叠加界面控件指针
     * UPROPERTY()宏使其受到Unreal垃圾回收系统的管理
     *
     * 属性特性说明：
     * 没有指定参数，使用默认设置：
     * - 不会被蓝图读写
     * - 可以被C++代码访问和修改
     * - 受垃圾回收保护
     *
     * 设计说明：
     * 1. TObjectPtr是UE5推荐的智能指针类型，用于安全地引用UObject对象
     * 2. UAuraUserWidget是自定义的用户控件类，包含游戏的具体UI逻辑
     * 3. 这个指针存储实际创建的控件实例，用于后续操作（如更新、隐藏等）
     *
     * 注意：这是一个运行时变量，在蓝图中不可见或编辑
     */
    UPROPERTY()
    TObjectPtr<UAuraUserWidget> OverlayWidget;

protected:
    /**
     * 重写父类的BeginPlay函数，在游戏开始时调用
     *
     * 功能说明：
     * 1. 初始化HUD和UI系统
     * 2. 创建并显示叠加界面
     * 3. 绑定游戏事件和UI更新
     *
     * 调用时机：
     * 在Actor完全初始化并准备好开始游戏后执行，晚于构造函数但早于Tick
     */
    virtual void BeginPlay() override;

private:
    /**
     * 叠加界面控件类引用（可编辑）
     * UPROPERTY宏使其在编辑器中可编辑，并支持蓝图系统
     *
     * 属性宏参数说明：
     * - EditAnywhere: 可以在编辑器中的任何地方编辑（默认值、实例、蓝图）
     * - Category = "UI": 在编辑器属性面板中归入"UI"分类，便于组织
     *
     * 类型说明：
     * TSubclassOf<UAuraUserWidget>: 类型安全的方式引用UAuraUserWidget或其子类
     * 这允许在蓝图中指定要实例化的具体控件类（如WBP_PlayerOverlay）
     *
     * 设计模式：
     * 使用"资产引用"模式，将具体实现与代码解耦，便于设计师调整UI
     */
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

}; 


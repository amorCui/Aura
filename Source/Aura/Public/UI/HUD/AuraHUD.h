// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"




class UOverlayWidgetController;
class UAuraUserWidget;
class UAbilitySystemComponent;
class UAttributeSet;

// 界面控制器参数结构体的前向声明，用于传递初始化参数
struct  FWidgetControllerParams;


/**
 * Aura游戏平视显示器（HUD）类
 * 继承自Unreal Engine的AHUD类，负责管理游戏的用户界面
 *
 * 功能扩展说明：
 * 相较于之前的基础版本，这个版本增加了：
 * 1. 界面控制器（WidgetController）的支持，实现了MVC（Model-View-Controller）模式
 * 2. 完整的初始化流程，分离了数据逻辑和视图显示
 * 3. 支持从游戏系统（PlayerState、AbilitySystem等）获取数据
 *
 * 设计模式：
 * 使用MVC模式分离关注点：
 * - Model: PlayerState、AttributeSet、AbilitySystemComponent（数据模型）
 * - View: UAuraUserWidget（视图，负责显示）
 * - Controller: UOverlayWidgetController（控制器，负责逻辑和绑定）
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD  // 继承自Unreal Engine的HUD基类
{
    // UE宏，生成反射和序列化所需的代码体
    GENERATED_BODY()

public:
    /**
     * 叠加界面控件指针
     * 这是实际的UI控件实例，负责在屏幕上显示游戏界面
     *
     * 设计说明：
     * 1. 在InitOverlay函数中创建和初始化
     * 2. 包含玩家状态信息（血条、法力条、技能栏等）
     * 3. 通过WidgetController与游戏数据绑定
     */
    UPROPERTY()
    TObjectPtr<UAuraUserWidget> OverlayWidget;

    /**
     * 获取或创建叠加界面控制器
     * 使用单例模式，确保只有一个控制器实例
     *
     * @param WCParams 界面控制器参数，包含初始化所需的所有数据
     * @return 返回叠加界面控制器指针，如果创建失败则返回nullptr
     *
     * 功能说明：
     * 1. 如果控制器尚未创建，则根据WidgetControllerClass创建新实例
     * 2. 如果控制器已存在，则使用现有实例
     * 3. 使用提供的参数设置控制器的数据源
     *
     * 设计模式：懒汉式单例模式（Lazy Singleton）
     */
    UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

    /**
     * 初始化叠加界面系统
     * 这是HUD初始化的核心函数，通常在游戏开始时调用
     *
     * @param PC 玩家控制器，提供输入和玩家控制信息
     * @param PS 玩家状态，包含玩家属性、等级等信息
     * @param ASC 能力系统组件，管理玩家的能力和效果
     * @param AS 属性集，包含玩家的具体属性值
     *
     * 初始化流程：
     * 1. 创建叠加界面控件（如果OverlayWidgetClass已设置）
     * 2. 创建叠加界面控制器（如果尚未创建）
     * 3. 将控件添加到游戏视口
     * 4. 建立数据绑定，使UI能够响应游戏状态变化
     *
     * 注意：这个函数通常在玩家角色完全初始化后调用
     */
    void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
    /**
     * 叠加界面控件类引用（可编辑）
     * 用于指定要创建的叠加界面控件类型
     *
     * 设计说明：
     * 1. 在编辑器中设置具体的控件蓝图类（如WBP_PlayerOverlay）
     * 2. 支持通过派生类创建不同的界面变体
     * 3. 与具体实现解耦，便于设计师调整UI
     */
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

    /**
     * 叠加界面控制器指针
     * 负责管理叠加界面的逻辑和数据绑定
     *
     * 功能说明：
     * 1. 监听游戏状态变化（属性变化、能力激活等）
     * 2. 更新UI控件显示
     * 3. 处理用户输入事件
     * 4. 作为游戏逻辑和UI显示之间的桥梁
     *
     * 注意：这是私有变量，通过GetOverlayWidgetController函数访问
     */
    UPROPERTY()
    TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

    /**
     * 叠加界面控制器类引用（可编辑）
     * 用于指定要创建的叠加界面控制器类型
     *
     * 设计说明：
     * 1. 在编辑器中设置具体的控制器类
     * 2. 支持通过派生类创建不同的控制器逻辑
     * 3. 可以创建不同类型的控制器（如玩家控制器、敌人控制器）
     */
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;



}; 
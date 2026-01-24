// Copyright Amor


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"




/**
 * 获取叠加界面控制器函数
 * 使用单例模式（懒汉式）创建或返回现有的叠加界面控制器
 *
 * @param WCParams 界面控制器参数结构体，包含初始化所需的所有组件引用
 * @return 返回叠加界面控制器指针，如果创建失败则返回nullptr
 *
 * 设计模式：懒汉式单例模式（Lazy Singleton）
 * 只在第一次请求时创建实例，后续请求返回同一实例
 */
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
    // 检查控制器是否已经创建，如果为空则创建新实例
    if (OverlayWidgetController == nullptr)
    {
        /**
         * 创建新的叠加界面控制器实例
         * NewObject<>: Unreal Engine的动态对象创建方法
         *
         * 参数说明：
         * - this: 外部对象（Outer），控制器的所有者（通常是这个HUD对象）
         * - OverlayWidgetControllerClass: 要创建的控制器类（在蓝图中指定的类）
         *
         * 注意：NewObject不会自动调用构造函数中的初始化逻辑，需要手动设置参数
         */
        OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);

        /**
         * 设置控制器的初始化参数
         * SetWidgetControllerParams(): 将游戏系统组件传递给控制器
         *
         * WCParams通常包含：
         * - PlayerController: 玩家控制器
         * - PlayerState: 玩家状态
         * - AbilitySystemComponent: 能力系统组件
         * - AttributeSet: 属性集
         *
         * 这些参数使控制器能够访问和监听游戏数据的变化
         */
        OverlayWidgetController->SetWidgetControllerParams(WCParams);

        /**
         * 调用叠加界面控制器的绑定回调到依赖项函数
         * 这行代码建立游戏属性变化与UI更新之间的连接，实现自动化的UI响应机制
         *
         * 功能说明：
         * 将控制器的回调函数绑定到Gameplay Ability System(GAS)的属性变化委托上，
         * 当游戏中的属性（如生命值、法力值）发生变化时，自动通知UI进行更新。
         *
         * 设计原理：
         * 1. GAS系统在属性变化时会触发委托（Delegate）
         * 2. 控制器将自己的成员函数绑定到这些委托上
         * 3. 当属性变化时，GAS调用绑定的函数，函数再广播给UI
         * 4. 实现"属性变化 → 控制器 → UI"的自动更新流程
         *
         * 调用时机的重要性：
         * 必须在以下条件满足后调用：
         * 1. 控制器已设置WidgetControllerParams（确保AbilitySystemComponent和AttributeSet有效）
         * 2. 在BroadCastInitialValues之前或之后都可以，但通常在之前
         * 3. 最好在UI绑定到控制器的委托之后调用，确保UI能立即响应变化
         */
        OverlayWidgetController->BindCallbacksToDependencies();
        // 注意：可以根据需要在这里添加控制器的额外初始化
    }

    // 返回控制器实例（无论是新创建的还是已有的）
    return OverlayWidgetController;
}

/**
 * 初始化叠加界面系统
 * 这是HUD初始化的核心函数，通常在游戏开始时调用
 *
 * @param PC 玩家控制器，提供输入和玩家控制信息
 * @param PS 玩家状态，包含玩家属性、等级等信息
 * @param ASC 能力系统组件，管理玩家的能力和效果
 * @param AS 属性集，包含玩家的具体属性值
 *
 * 函数执行流程：
 * 1. 安全检查：确保所需的类引用已设置
 * 2. 创建叠加界面控件
 * 3. 创建或获取叠加界面控制器
 * 4. 建立控制器与控件的关联
 * 5. 将控件添加到游戏视口
 */
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
    /**
     * 安全检查：确保叠加界面控件类已设置
     * checkf: 条件检查宏，如果条件为false则触发断言并显示错误信息
     *
     * 参数说明：
     * - 条件：OverlayWidgetClass != nullptr
     * - 错误信息：提示用户在蓝图中设置OverlayWidgetClass
     *
     * 这个检查防止了运行时崩溃，提供了清晰的错误指导
     */
    checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));

    /**
     * 安全检查：确保叠加界面控制器类已设置
     * 同样使用checkf宏，提供清晰的错误信息
     */
    checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class Unitialized, please fill out BP_AuraHUD"));

    /**
     * 创建叠加界面控件实例
     * CreateWidget<>: 创建用户控件实例的标准方法
     *
     * 参数说明：
     * - GetWorld(): 获取当前游戏世界上下文
     * - OverlayWidgetClass: 要实例化的控件类（在蓝图中指定）
     *
     * 注意：这里先创建UUserWidget基类，然后转换为具体类型
     * 这样做是为了处理模板实例化的编译问题
     */
    UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);

    /**
     * 将创建的控件转换为具体的UAuraUserWidget类型
     * Cast<>: 安全的类型转换，如果转换失败则返回nullptr
     *
     * 这个转换是安全的，因为OverlayWidgetClass应该是UAuraUserWidget或其子类
     */
    OverlayWidget = Cast<UAuraUserWidget>(Widget);

    /**
     * 创建界面控制器参数结构体
     * FWidgetControllerParams: 包含初始化控制器所需的所有组件引用
     *
     * 参数说明：
     * - PC: 玩家控制器（处理输入、摄像机等）
     * - PS: 玩家状态（存储玩家等级、经验、属性等）
     * - ASC: 能力系统组件（管理能力和效果）
     * - AS: 属性集（存储具体的属性值）
     *
     * 这个结构体将所有相关的游戏系统组件打包，便于传递
     */
    const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

    /**
     * 获取或创建叠加界面控制器
     * GetOverlayWidgetController(): 单例模式的控制器获取函数
     *
     * 函数内部会：
     * 1. 检查控制器是否已存在
     * 2. 如果不存在，创建新实例并设置参数
     * 3. 返回控制器指针
     */
    UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

    /**
     * 将控制器设置给叠加界面控件
     * SetWidgetController(): 建立控件与控制器的关联
     *
     * 功能说明：
     * 1. 控制器监听游戏数据变化
     * 2. 控件通过控制器获取和显示数据
     * 3. 实现了MVC模式中的控制器-视图连接
     *
     * 注意：控件内部可能会绑定到控制器的数据源和事件
     */
    OverlayWidget->SetWidgetController(WidgetController);

    /**
     * 调用界面控制器的广播初始值函数
     * 这行代码触发属性初始值的广播，确保UI在显示时具有正确的初始状态
     *
     * 执行流程：
     * 1. WidgetController从属性集获取当前属性值
     * 2. 通过委托系统将属性值广播给所有监听的UI元素
     * 3. UI元素接收到初始值后更新显示
     *
     * 典型使用场景：
     * 1. 在HUD初始化时调用，确保UI显示正确的初始值
     * 2. 在角色重生或重置时调用，更新UI到新的初始状态
     * 3. 在界面重新绑定时调用，确保UI与新角色的属性同步
     *
     * 技术细节：
     * 1. BroadCastInitialValues()是UOverlayWidgetController类中重写的虚函数
     * 2. 该函数内部调用委托的Broadcast()方法，触发所有绑定的回调
     * 3. 这是一个多播委托，可以同时通知多个UI元素
     */
    WidgetController->BroadCastInitialValues();

    /**
     * 将叠加界面控件添加到游戏视口
     * AddToViewport(): 显示控件，使其成为游戏界面的一部分
     *
     * 注意：这里没有指定ZOrder（渲染层级），使用默认值
     * 如果需要控件在其他UI之上或之下，可以指定ZOrder值
     */
    OverlayWidget->AddToViewport();

}

/**
 * 完整初始化流程总结：
 *
 * 1. 安全检查（checkf）
 *    - 确保所有必需的类引用已设置
 *    - 提供清晰的错误信息指导
 *
 * 2. 创建界面控件（CreateWidget）
 *    - 根据蓝图设置的类创建控件实例
 *    - 类型转换为具体的UAuraUserWidget
 *
 * 3. 准备控制器参数（FWidgetControllerParams）
 *    - 收集所有相关的游戏系统组件
 *    - 封装成结构体便于传递
 *
 * 4. 获取或创建控制器（GetOverlayWidgetController）
 *    - 使用单例模式确保只有一个控制器实例
 *    - 设置控制器参数，使其能够访问游戏数据
 *
 * 5. 建立控件与控制器关联（SetWidgetController）
 *    - 实现MVC架构，分离数据和显示
 *    - 控件通过控制器响应游戏状态变化
 *
 * 6. 显示界面（AddToViewport）
 *    - 将控件添加到游戏视口
 *    - 用户现在可以看到并交互
 *
 * 7. 后续操作（可选）
 *    - 初始化控件状态
 *    - 绑定额外的事件
 *    - 播放动画或效果
 */

 



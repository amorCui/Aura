// Copyright Amor


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"

// AAuraHUD类的BeginPlay函数实现
// 此函数在HUD（平视显示器）开始游戏时调用，用于初始化用户界面

void AAuraHUD::BeginPlay()
{
    /**
     * 调用父类的BeginPlay函数，确保基础初始化完成
     * 这是Unreal Engine的标准做法，确保继承链中的所有初始化逻辑正确执行
     * 父类可能会初始化HUD的基础状态、设置默认值等
     */
    Super::BeginPlay();

    /**
     * 检查叠加界面（Overlay Widget）类是否已设置（不为空）
     * OverlayWidgetClass是一个TSubclassOf<UAuraUserWidget>类型的变量，
     * 通常在编辑器中指定或在代码中动态设置
     *
     * 安全机制：避免尝试创建未定义的控件类
     */
    if (OverlayWidgetClass)
    {
        /**
         * 创建叠加界面控件实例
         * CreateWidget<>: Unreal Engine的控件创建方法，用于动态生成用户界面控件
         *
         * 模板参数：
         * - UAuraUserWidget: 要创建的控件类型（自定义用户控件）
         *
         * 函数参数：
         * - GetWorld(): 获取当前游戏世界上下文，控件需要关联到特定的游戏世界
         * - OverlayWidgetClass: 要实例化的控件类（蓝图类或C++类）
         *
         * 返回值：
         * - 返回新创建的控件实例指针，类型为UAuraUserWidget*
         */
        OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);

        /**
         * 检查控件是否成功创建
         * 创建可能失败的情况：
         * 1. OverlayWidgetClass引用无效
         * 2. 内存不足
         * 3. 游戏世界无效
         */
        if (OverlayWidget)
        {
            /**
             * 将控件添加到游戏视口（Viewport）
             * AddToViewport(): 将控件显示在屏幕上，成为游戏界面的一部分
             *
             * 功能说明：
             * 1. 使控件在屏幕上可见并可交互
             * 2. 控件的渲染层级由ZOrder参数控制（默认添加到最后）
             * 3. 控件会随着游戏运行每帧更新
             *
             * 可选参数：
             * - ZOrder: 渲染层级，数值越大显示在越上层
             *
             * 注意：添加到视口的控件会在切换关卡或手动移除时被销毁
             */
            OverlayWidget->AddToViewport();
        }
      
    }

}

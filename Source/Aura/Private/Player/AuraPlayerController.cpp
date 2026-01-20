// Copyright Amor

#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubSystems.h"

/**
 * AAuraPlayerController 构造函数
 * 初始化玩家控制器的基本属性
 */
AAuraPlayerController::AAuraPlayerController()
{
    // 启用网络复制功能，使该对象在网络游戏中同步
    // bReplicates = true 表示这个Actor将在网络游戏中被复制到所有客户端
    bReplicates = true;

    // 可以在此处添加更多初始化逻辑，例如：
    // - 输入设置
    // - 默认相机设置
    // - 网络相关设置
}

/**
 * BeginPlay 函数
 * 当控制器开始游戏时调用
 * 这里是设置玩家输入和UI模式的理想位置
 */
void AAuraPlayerController::BeginPlay()
{
    // 首先调用父类的BeginPlay，确保基础初始化完成
    Super::BeginPlay();

    // 检查AuraContext是否有效，如果为空则触发断言错误
    // AuraContext应该是一个UInputMappingContext类型的变量，用于定义输入映射
    // 需要在头文件中声明并在其他地方初始化
    check(AuraContext); // 如果AuraContext为nullptr，游戏将崩溃并显示错误信息

    // 获取增强输入本地玩家子系统
    // 这是处理增强输入系统的子系统
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    // 检查子系统是否成功获取，失败则触发断言
    check(Subsystem);

    // 将输入映射上下文添加到子系统中，优先级为0
    // 优先级数字越小，优先级越高（0是最高的）
    Subsystem->AddMappingContext(AuraContext, 0);

    // 设置鼠标光标在游戏中可见
    // 这对于有UI界面的游戏很重要
    bShowMouseCursor = true;

    // 设置默认的鼠标光标样式为系统默认样式
    DefaultMouseCursor = EMouseCursor::Default;

    // 创建游戏和UI混合输入模式
    // 这种模式允许同时处理游戏输入和UI输入
    FInputModeGameAndUI InputModeData;

    // 设置鼠标锁定行为：不锁定鼠标到视口
    // 这意味着鼠标可以自由移动到游戏窗口外
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    // 设置输入捕获期间不隐藏光标
    // 当玩家点击UI时，鼠标光标不会消失
    InputModeData.SetHideCursorDuringCapture(false);

    // 应用设置的输入模式
    SetInputMode(InputModeData);

    // 可以在此处添加更多BeginPlay逻辑，例如：
    // - 初始化玩家状态
    // - 绑定输入事件
    // - 加载玩家数据
    // - 设置相机控制
}
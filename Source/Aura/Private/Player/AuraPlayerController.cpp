// Copyright ：Amor

// 包含AuraPlayerController的头文件
#include "Player/AuraPlayerController.h"

// 包含增强输入系统相关的头文件
// EnhancedInputSubSystems.h：增强输入子系统
// EnhancedInputComponent.h：增强输入组件，用于绑定输入事件
#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

// 注意：还需要包含InputAction.h，因为MoveAction是UInputAction类型
// 假设MoveAction在头文件中已声明为TObjectPtr<UInputAction>类型

/**
 * AAuraPlayerController 构造函数
 * 初始化玩家控制器的基本属性
 */
AAuraPlayerController::AAuraPlayerController()
{
    // 启用网络复制功能，使该对象在网络游戏中同步
    // bReplicates = true 表示这个Actor将在网络游戏中被复制到所有客户端
    bReplicates = true;

}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    CursorTrace();
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
    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

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
}

/**
 * SetupInputComponent 函数
 * 用于设置输入组件并绑定输入事件
 * 此函数在APlayerController中被调用，用于初始化输入系统
 */
void AAuraPlayerController::SetupInputComponent()
{
    // 调用父类的SetupInputComponent以进行基础设置
    // 注释掉这行可能是因为需要完全自定义输入，不使用父类的绑定
    Super::SetupInputComponent();

    // 将InputComponent转换为增强输入组件
    // CastChecked：安全的类型转换，如果转换失败会触发断言
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    // 绑定移动输入事件
    // MoveAction：预定义的UInputAction资产，表示移动输入
    // ETriggerEvent::Triggered：当输入持续触发时调用（例如按住按键时）
    // this：调用此控制器实例的Move函数
    EnhancedInputComponent->BindAction(
        MoveAction,                     // 输入动作
        ETriggerEvent::Triggered,       // 触发事件类型
        this,                           // 执行对象
        &AAuraPlayerController::Move    // 回调函数
    );


}

/**
 * Move 函数
 * 处理角色移动的输入回调
 * @param InputActionValue 输入动作的值，包含输入方向和强度
 */
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
    // 从输入动作值中获取2D向量（通常来自键盘WASD或手柄摇杆）
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

    // 获取控制器的当前旋转（看向的方向）
    const FRotator Rotation = GetControlRotation();

    // 只使用Yaw（水平旋转）创建新的旋转，忽略Pitch（俯仰）和Roll（翻滚）
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    // 根据YawRotation计算前向方向向量（前进/后退方向）
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    // 根据YawRotation计算右向方向向量（左右平移方向）
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    // 获取此控制器当前控制的Pawn（角色）
    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        // 根据输入向量的Y分量（前向/后向）应用前向移动
        // InputAxisVector.Y：正数向前，负数向后
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);

        // 根据输入向量的X分量（左/右）应用右向移动
        // InputAxisVector.X：正数向右，负数向左
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);

        // 注意：AddMovementInput会累加移动输入，直到下一帧被清除
        // 这使得按住按键可以持续移动
    }

}

void AAuraPlayerController::CursorTrace()
{
    // 执行射线检测，获取光标下的碰撞结果
    // ECC_Visibility: 使用可见性碰撞通道
    // false: 不忽略任何碰撞（不使用复杂碰撞）
    FHitResult CursorHit;
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

    // 如果没有击中任何物体，则直接返回
    if (!CursorHit.bBlockingHit) return;

    // 保存上一帧检测到的Actor到LastActor
    // 将当前帧检测到的Actor存储到ThisActor（临时变量）
    LastActor = ThisActor;

    // 将击中的Actor转换为IEnemyInterface接口指针
    // 如果Actor实现了IEnemyInterface接口，则返回有效指针，否则返回nullptr
    ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

    /**
     * 鼠标射线检测逻辑分析（5种情况）：
     *
     * A. LastActor为空 && ThisActor为空
     *     - 上一帧和当前帧都没有检测到敌人，什么都不做
     *
     * B. LastActor为空 && ThisActor有效
     *     - 上一帧没有敌人，当前帧检测到敌人，高亮当前敌人
     *
     * C. LastActor有效 && ThisActor为空
     *     - 上一帧有敌人，当前帧没有检测到敌人，取消上一帧敌人的高亮
     *
     * D. 两个Actor都有效，但是LastActor != ThisActor
     *     - 上一帧和当前帧检测到不同的敌人，取消上一个敌人的高亮，高亮当前敌人
     *
     * E. 两个Actor都有效，并且LastActor == ThisActor
     *     - 连续两帧检测到同一个敌人，什么都不做（避免重复高亮）
     */

     // 情况A和B：LastActor为空
    if (LastActor == nullptr)
    {
        // 情况B：LastActor为空，ThisActor有效
        if (ThisActor != nullptr)
        {
            // Case B: 高亮ThisActor（新检测到的敌人）
            ThisActor->HighlightActor();  // 注意：函数名应该是HighlightActor，不是HightlightActor
        }
        // 情况A：LastActor为空，ThisActor为空 - 什么都不做
        else
        {
            // Case A: 没有敌人需要处理
        }
    }
    // 情况C、D和E：LastActor有效
    else
    {
        // 情况C：LastActor有效，ThisActor为空
        if (ThisActor == nullptr)
        {
            // Case C: 取消LastActor的高亮（敌人离开光标范围）
            LastActor->UnHighlightActor();  // 注意：函数名应该是UnHighlightActor，不是UnHightlightActor
        }
        // 情况D和E：ThisActor有效
        else
        {
            // 情况D：两个Actor不同
            if (LastActor != ThisActor)
            {
                // Case D: 高亮新敌人，取消旧敌人的高亮
                ThisActor->HighlightActor();
                LastActor->UnHighlightActor();
            }
            // 情况E：两个Actor相同
            else
            {
                // Case E: 同一个敌人，避免重复操作
                // 可以选择更新高亮持续时间或其他状态，但这里什么都不做
            }
        }
    }
}

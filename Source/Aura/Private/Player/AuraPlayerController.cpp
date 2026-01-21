// Copyright Amor

#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubSystems.h"
#include "EnhancedInputComponent.h"

// 包含敌人接口定义，用于交互检测和敌人高亮功能
#include "Interaction/EnemyInterface.h"

/**
 * AAuraPlayerController 构造函数
 * 初始化玩家控制器的基本属性，设置默认参数
 * 在对象创建时自动调用，用于初始状态的设置
 */
AAuraPlayerController::AAuraPlayerController()
{
    /**
     * 启用网络复制功能，使该对象在网络游戏中同步
     * bReplicates = true 表示这个Actor将在网络游戏中被复制到所有客户端
     * 这对于多人游戏至关重要，确保所有玩家看到的控制器状态一致
     */
    bReplicates = true;

}

/**
 * 玩家每帧更新函数
 * 重写自APlayerController，每帧都会被游戏引擎调用
 * @param DeltaTime 距离上一帧的时间间隔（秒），用于实现与帧率无关的逻辑
 * 例如：移动速度应乘以DeltaTime来确保在不同帧率下移动距离一致
 */
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    // 调用父类的PlayerTick，确保基础更新逻辑正常执行
    // 父类可能处理了输入缓冲、摄像机更新等重要功能
    Super::PlayerTick(DeltaTime);

    // 每帧执行光标准踪，检测光标下的交互对象
    // 这确保玩家移动光标时能实时响应交互变化
    CursorTrace();
}

/**
 * BeginPlay 函数
 * 当控制器开始游戏时调用，在Actor完全初始化并准备好开始游戏后执行
 * 这里是设置玩家输入、UI模式和游戏初始状态的理想位置
 */
void AAuraPlayerController::BeginPlay()
{
    // 首先调用父类的BeginPlay，确保基础初始化完成
    // 父类可能设置了输入模式、摄像机等重要组件
    Super::BeginPlay();

    /**
     * 检查AuraContext是否有效，如果为空则触发断言错误
     * check()宏在开发构建中有效，如果条件为false则崩溃并显示调用栈
     * AuraContext应该是一个UInputMappingContext类型的变量，用于定义输入映射
     * 此检查确保在尝试使用输入上下文之前它已被正确设置
     */
    check(AuraContext);

    /**
     * 获取增强输入本地玩家子系统
     * 这是处理增强输入系统的子系统，每个本地玩家都有一个
     * GetLocalPlayer()返回当前控制器所属的本地玩家对象
     * GetSubsystem<>()是模板函数，用于获取指定类型的子系统
     */
    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    // 再次检查子系统是否成功获取，失败则触发断言
    // 确保增强输入系统已正确初始化和可用
    check(Subsystem);

    /**
     * 将输入映射上下文添加到子系统中，优先级为0
     * 优先级数字越小，优先级越高（0是最高的）
     * 这意味着AuraContext中的输入映射将覆盖优先级较低的上下文
     * 多个上下文可以叠加，高优先级的上下文会覆盖低优先级的相同输入绑定
     */
    Subsystem->AddMappingContext(AuraContext, 0);

    /**
     * 设置鼠标光标在游戏中可见
     * 这对于有UI界面的游戏很重要，允许玩家使用鼠标与UI交互
     * 在纯动作游戏或控制台游戏中，可能不需要显示鼠标光标
     */
    bShowMouseCursor = true;

    /**
     * 设置默认的鼠标光标样式为系统默认样式
     * EMouseCursor::Default使用操作系统默认的鼠标光标样式
     * 其他选项包括：文本输入光标、手形光标、禁止符号等
     */
    DefaultMouseCursor = EMouseCursor::Default;

    /**
     * 创建游戏和UI混合输入模式
     * 这种模式允许同时处理游戏输入和UI输入，是RPG/MMO游戏的常见设置
     * 玩家可以使用鼠标与UI交互，同时使用键盘/手柄控制角色
     */
    FInputModeGameAndUI InputModeData;

    /**
     * 设置鼠标锁定行为：不锁定鼠标到视口
     * EMouseLockMode::DoNotLock意味着鼠标可以自由移动到游戏窗口外
     * 其他选项：
     * - LockOnCapture：仅在捕获输入时锁定（如射击时）
     * - LockAlways：始终锁定在窗口中心（常见于FPS游戏）
     */
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    /**
     * 设置输入捕获期间不隐藏光标
     * 当玩家点击UI时，鼠标光标不会消失
     * 这对于需要精确鼠标控制的UI交互很重要
     */
    InputModeData.SetHideCursorDuringCapture(false);

    /**
     * 应用设置的输入模式到当前控制器
     * SetInputMode()是APlayerController的方法，用于应用输入模式配置
     * 输入模式决定了控制器如何处理不同类型的输入（游戏、UI等）
     */
    SetInputMode(InputModeData);
}

/**
 * SetupInputComponent 函数
 * 用于设置输入组件并绑定输入事件
 * 此函数在APlayerController初始化时被调用，用于初始化输入系统
 * InputComponent负责将原始输入转换为游戏动作
 */
void AAuraPlayerController::SetupInputComponent()
{
    /**
     * 调用父类的SetupInputComponent以进行基础设置
     * 注释掉这行可能是因为需要完全自定义输入，不使用父类的绑定
     * 但通常建议保留这行，以确保基础输入功能正常工作
     */
    Super::SetupInputComponent();

    /**
     * 将InputComponent转换为增强输入组件
     * CastChecked：安全的类型转换，如果转换失败会触发断言
     * 增强输入组件提供了更灵活的输入绑定系统，支持复杂的输入处理
     * InputComponent是APlayerController的成员，用于处理所有输入
     */
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    /**
     * 绑定移动输入事件
     * MoveAction：预定义的UInputAction资产，表示移动输入
     * ETriggerEvent::Triggered：当输入持续触发时调用（例如按住按键时）
     * this：调用此控制器实例的Move函数
     * 绑定将指定的输入动作与特定的回调函数关联起来
     */
    EnhancedInputComponent->BindAction(
        MoveAction,                     // 输入动作，表示"移动"操作
        ETriggerEvent::Triggered,       // 触发事件类型：持续触发（按住按键）
        this,                           // 执行对象：当前控制器实例
        &AAuraPlayerController::Move    // 回调函数：当输入触发时调用的函数
    );
    // 注意：可以在此处添加更多输入绑定，如攻击、技能、菜单等
}

/**
 * Move 函数
 * 处理角色移动的输入回调，当移动输入被触发时调用
 * @param InputActionValue 输入动作的值，包含输入方向和强度信息
 * 对于2D移动输入，通常包含X（左右）和Y（前后）分量，值范围通常在-1到1之间
 */
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
    /**
     * 从输入动作值中获取2D向量（通常来自键盘WASD或手柄摇杆）
     * Get<FVector2D>()是模板方法，获取特定类型的输入值
     * 返回值：
     * - X分量：水平方向输入（左/右），-1表示左，1表示右
     * - Y分量：垂直方向输入（前/后），-1表示后，1表示前
     */
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

    /**
     * 获取控制器的当前旋转（看向的方向）
     * GetControlRotation()返回控制器的旋转角度（偏航Yaw、俯仰Pitch、翻滚Roll）
     * 这对于计算基于摄像机方向的移动非常重要
     */
    const FRotator Rotation = GetControlRotation();

    /**
     * 只使用Yaw（水平旋转）创建新的旋转，忽略Pitch（俯仰）和Roll（翻滚）
     * 这确保了移动是相对于水平地面的，不会因为摄像机俯仰而向上/向下移动
     * FRotator构造函数的参数顺序为：(Pitch, Yaw, Roll)
     */
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    /**
     * 根据YawRotation计算前向方向向量（前进/后退方向）
     * FRotationMatrix：将旋转转换为矩阵，便于获取方向向量
     * GetUnitAxis(EAxis::X)：获取X轴单位向量，在UE坐标系中通常是前向
     * UE坐标系：X=前，Y=右，Z=上
     */
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    /**
     * 根据YawRotation计算右向方向向量（左右平移方向）
     * GetUnitAxis(EAxis::Y)：获取Y轴单位向量，在UE坐标系中通常是右向
     */
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    /**
     * 获取此控制器当前控制的Pawn（角色）
     * GetPawn<APawn>()：获取控制器控制的Pawn对象，并进行类型转换
     * Pawn是玩家直接控制的游戏角色或载具
     */
    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        /**
         * 根据输入向量的Y分量（前向/后向）应用前向移动
         * AddMovementInput()：将移动输入添加到Pawn的移动组件中
         * 参数1：移动方向（单位向量）
         * 参数2：移动强度（输入值，通常范围-1到1）
         * InputAxisVector.Y：正数向前，负数向后
         * 注意：实际的移动计算由Pawn的移动组件在物理更新时处理
         */
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);

        /**
         * 根据输入向量的X分量（左/右）应用右向移动
         * InputAxisVector.X：正数向右，负数向左
         * 这实现了基于摄像机方向的左右平移
         */
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);

        // 注意：AddMovementInput会累加移动输入，直到下一帧被清除
        // 这使得按住按键可以持续移动，而无需每帧重新绑定输入
        // 实际的移动速度和加速度由Pawn的MovementComponent控制
    }
    // 如果没有控制任何Pawn，则移动输入被忽略（可能在角色死亡或切换时发生）
}

/**
 * CursorTrace 函数
 * 光标准踪函数 - 用于检测光标下的交互对象
 * 通过射线检测从屏幕光标位置向游戏世界发射射线，检测命中的对象
 * 主要用于高亮敌人、显示交互提示等交互功能
 * 此函数在PlayerTick中每帧调用，确保实时响应
 */
void AAuraPlayerController::CursorTrace()
{
    /**
     * 执行射线检测，获取光标下的碰撞结果
     * FHitResult：射线检测结果结构，包含命中的详细信息
     * ECC_Visibility: 使用可见性碰撞通道，通常用于可见物体的检测
     * false: 不使用复杂碰撞（不进行三角形级别的精确检测，性能更好）
     * true: 会忽略简单的碰撞体，使用更精确但更耗性能的复杂碰撞
     * GetHitResultUnderCursor()是APlayerController的方法，从光标位置发射射线
     */
    FHitResult CursorHit;
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

    // 如果没有击中任何物体（射线没有碰到任何碰撞体），则直接返回
    // bBlockingHit为true表示射线被碰撞体阻挡，为false表示没有命中或穿透
    if (!CursorHit.bBlockingHit) return;

    /**
     * 保存上一帧检测到的Actor到LastActor
     * 这是状态追踪的重要部分，用于检测交互对象的变化
     * LastActor用于追踪前一帧光标下的交互对象
     */
    LastActor = ThisActor;

    /**
     * 将击中的Actor转换为IEnemyInterface接口指针
     * Cast<IEnemyInterface>()：尝试将Actor转换为指定接口类型
     * 如果Actor实现了IEnemyInterface接口，则返回有效指针，否则返回nullptr
     * 这种设计使得系统可以处理任何实现了敌人接口的对象，而不关心具体类型
     * GetActor()返回射线击中的AActor对象
     */
    ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

    /**
     * 鼠标射线检测逻辑分析（5种情况）：
     *
     * A. LastActor为空 && ThisActor为空
     *     - 上一帧和当前帧都没有检测到敌人，什么都不做
     *     - 这是最常见的情况，光标在空白区域移动
     *
     * B. LastActor为空 && ThisActor有效
     *     - 上一帧没有敌人，当前帧检测到敌人，高亮当前敌人
     *     - 光标从空白区域移动到敌人身上
     *
     * C. LastActor有效 && ThisActor为空
     *     - 上一帧有敌人，当前帧没有检测到敌人，取消上一帧敌人的高亮
     *     - 光标从敌人身上移动到空白区域
     *
     * D. 两个Actor都有效，但是LastActor != ThisActor
     *     - 上一帧和当前帧检测到不同的敌人，取消上一个敌人的高亮，高亮当前敌人
     *     - 光标从一个敌人身上移动到另一个敌人身上
     *
     * E. 两个Actor都有效，并且LastActor == ThisActor
     *     - 连续两帧检测到同一个敌人，什么都不做（避免重复高亮）
     *     - 光标停留在同一个敌人身上
     */

     // 情况A和B：LastActor为空（上一帧没有检测到敌人）
    if (LastActor == nullptr)
    {
        // 情况B：LastActor为空，ThisActor有效（新检测到敌人）
        if (ThisActor != nullptr)
        {
            // Case B: 高亮ThisActor（新检测到的敌人）
            // 调用敌人的高亮函数，通常会使敌人发光或显示轮廓
            // 注意：函数名应该是HighlightActor，不是HightlightActor
            ThisActor->HighlightActor();
        }
        // 情况A：LastActor为空，ThisActor为空（光标在空白区域）
        else
        {
            // Case A: 没有敌人需要处理，保持当前状态
            // 这是空操作分支，可以省略，但为了逻辑完整性保留
        }
    }
    // 情况C、D和E：LastActor有效（上一帧检测到了敌人）
    else
    {
        // 情况C：LastActor有效，ThisActor为空（敌人离开光标范围）
        if (ThisActor == nullptr)
        {
            // Case C: 取消LastActor的高亮（敌人离开光标范围）
            // 调用敌人的取消高亮函数，恢复敌人正常外观
            // 注意：函数名应该是UnHighlightActor，不是UnHightlightActor
            LastActor->UnHighlightActor();
        }
        // 情况D和E：ThisActor有效（当前帧也检测到了敌人）
        else
        {
            // 情况D：两个Actor不同（光标从一个敌人移动到另一个敌人）
            if (LastActor != ThisActor)
            {
                // Case D: 高亮新敌人，取消旧敌人的高亮
                // 先高亮新敌人，再取消旧敌人的高亮
                // 顺序重要：确保视觉上不会出现两个敌人都高亮的情况
                ThisActor->HighlightActor();
                LastActor->UnHighlightActor();
            }
            // 情况E：两个Actor相同（光标停留在同一个敌人身上）
            else
            {
                // Case E: 同一个敌人，避免重复操作
                // 可以选择更新高亮持续时间或其他状态，但这里什么都不做
                // 优化：可以添加计时器或状态检查，避免每帧都调用相同的接口
            }
        }
    }
}
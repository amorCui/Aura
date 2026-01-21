// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/EnemyInterface.h"

#include "AuraPlayerController.generated.h"

// 前向声明输入系统相关类，避免不必要的头文件包含
// 前向声明可以减少编译依赖，加快编译速度
class UInputMappingContext;   // 输入映射上下文类的前向声明，用于定义输入绑定
class UInputAction;           // 输入动作类的前向声明，表示单个输入操作
struct FInputActionValue;     // 输入动作值结构体的前向声明，包含输入数据



/**
 * Aura玩家控制器类
 * 负责处理玩家输入和控制逻辑，是玩家与游戏世界交互的主要接口
 * 继承自Unreal Engine的标准PlayerController类
 *
 * 主要功能：
 * 1. 处理玩家输入（移动、攻击、技能等）
 * 2. 管理光标追踪和交互对象检测
 * 3. 控制玩家视角和摄像机
 */
UCLASS()  // UCLASS宏，向Unreal反射系统注册该类，使其可用于蓝图、序列化等系统功能
class AURA_API AAuraPlayerController : public APlayerController
{
    // UE宏，生成反射和序列化所需的代码体
    // 必须放在类定义的顶部，紧接着类名之后
    // 这个宏展开的代码实现了UE的反射系统所需的各种功能
    GENERATED_BODY()

public:
    // 构造函数声明
    // 用于初始化控制器对象的默认状态
    AAuraPlayerController();

    /**
     * 玩家每帧更新函数
     * 重写自APlayerController，每帧都会被调用
     * @param DeltaTime 距离上一帧的时间间隔（秒），用于实现与帧率无关的逻辑
     */
    virtual void PlayerTick(float DeltaTime) override;

protected:
    /**
     * 游戏开始时调用
     * 重写自AActor，在控制器完全初始化并准备开始游戏时执行
     * 用于执行游戏开始时的初始化逻辑
     */
    virtual void BeginPlay() override;

    /**
     * 设置输入组件函数
     * 重写自APlayerController，用于设置输入组件并绑定输入事件
     * 此函数在控制器初始化时被调用，是绑定输入操作的理想位置
     */
    virtual void SetupInputComponent() override;

private:
    /**
     * 输入映射上下文，用于定义玩家输入与游戏操作的映射关系
     * UPROPERTY宏使该属性在编辑器中可编辑，并支持Unreal Engine的反射系统
     * EditAnywhere: 可以在蓝图编辑器中的任何地方（默认值、实例）编辑此属性
     * Category = "Input": 在编辑器属性面板中归入"Input"分类，便于查找和编辑
     * TObjectPtr是UE5的智能指针类型，用于安全地引用UObject对象，自动处理引用计数
     */
    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputMappingContext> AuraContext;

    /**
     * 移动输入动作，用于处理角色的移动输入
     * 在蓝图中可以分配具体的输入动作资产（如键盘WASD或手柄摇杆）
     * 输入动作定义了触发条件（按下、释放、持续按压等）和输入值处理方式
     */
    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    /**
     * 移动处理函数，响应移动输入事件
     * @param InputActionValue 输入动作值的常量引用，包含输入方向和强度信息
     * 结构包含：
     * - 输入类型（布尔值、一维值、二维向量等）
     * - 输入强度（如摇杆的偏移量或按键的压力值）
     * - 当前输入状态
     */
    void Move(const FInputActionValue& InputActionValue);

    /**
     * 光标准踪函数 - 用于检测光标下的交互对象
     * 通过射线检测从屏幕光标位置向游戏世界发射射线，检测命中的对象
     * 主要用于高亮敌人、显示交互提示等交互功能
     */
    void CursorTrace();

    /**
     * 上一帧检测到的Actor接口指针
     * 用于追踪前一帧光标下的交互对象，实现对象进出状态的检测
     * IEnemyInterface* 类型表示实现了敌人接口的对象
     */
    IEnemyInterface* LastActor;

    /**
     * 当前帧检测到的Actor接口指针
     * 存储当前帧光标下检测到的交互对象
     * 通过比较LastActor和ThisActor，可以判断交互对象的变化
     */
    IEnemyInterface* ThisActor;

};  
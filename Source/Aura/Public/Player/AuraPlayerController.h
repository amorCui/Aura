// Copyright Amor

// 预编译头指令，防止头文件被多次包含
#pragma once

// 包含Unreal Engine的核心头文件
#include "CoreMinimal.h"
// 包含Unreal Engine的PlayerController基类
#include "GameFramework/PlayerController.h"
// 包含自动生成的代码头文件
#include "AuraPlayerController.generated.h"

// 前向声明输入系统相关类，避免不必要的头文件包含
// 前向声明可以减少编译依赖，加快编译速度
class UInputMappingContext;   // 输入映射上下文类的前向声明
class UInputAction;           // 输入动作类的前向声明
struct FInputActionValue;     // 输入动作值结构体的前向声明

/**
 * Aura玩家控制器类
 * 负责处理玩家输入和控制逻辑
 * 继承自Unreal Engine的标准PlayerController类
 */
UCLASS()  // UCLASS宏，向Unreal反射系统注册该类，使其可用于蓝图、序列化等
class AURA_API AAuraPlayerController : public APlayerController
{
    // UE宏，生成反射和序列化所需的代码体
    // 必须放在类定义的顶部，紧接着类名之后
    GENERATED_BODY()

public:
    // 构造函数声明
    AAuraPlayerController();

protected:
    // 重写BeginPlay函数，在控制器开始游戏时调用
    virtual void BeginPlay() override;

    // 重写SetupInputComponent函数，用于设置输入组件并绑定输入事件
    // 此函数在控制器初始化时被调用，是绑定输入操作的理想位置
    virtual void SetupInputComponent() override;

private:
    // 输入映射上下文，用于定义玩家输入与游戏操作的映射关系
    // UPROPERTY宏使该属性在编辑器中可编辑，并支持Unreal Engine的反射系统
    // EditAnywhere: 可以在蓝图编辑器中的任何地方（默认值、实例）编辑此属性
    // Category = "Input": 在编辑器属性面板中归入"Input"分类
    // TObjectPtr是UE5的智能指针类型，用于安全地引用UObject对象
    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputMappingContext> AuraContext;

    // 移动输入动作，用于处理角色的移动输入
    // 在蓝图中可以分配具体的输入动作资产（如键盘WASD或手柄摇杆）
    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    // 移动处理函数，响应移动输入事件
    // const FInputActionValue&: 输入动作值的常量引用，包含输入方向和强度信息
    void Move(const FInputActionValue& InputActionValue);

    // 可以在此处添加更多私有成员，例如：
    // - 其他输入动作（攻击、跳跃、交互等）
    // - 输入相关状态变量
    // - 输入缓冲逻辑
};
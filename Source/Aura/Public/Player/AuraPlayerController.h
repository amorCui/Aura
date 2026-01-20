// Copyright Amor

// 预编译头指令，防止头文件被多次包含
#pragma once

// 包含Unreal Engine的核心头文件
#include "CoreMinimal.h"
// 包含Unreal Engine的PlayerController基类
#include "GameFramework/PlayerController.h"
// 包含自动生成的代码头文件
#include "AuraPlayerController.generated.h"

class UInputMappingContext;

/**
 * Aura玩家控制器类
 * 负责处理玩家输入和控制逻辑
 * 继承自Unreal Engine的标准PlayerController类
 */
UCLASS()														// UCLASS宏，向Unreal反射系统注册该类
class AURA_API AAuraPlayerController : public APlayerController // 继承自APlayerController
{
	GENERATED_BODY() // UE宏，生成反射和序列化所需的代码体

public:
	// 构造函数声明
	AAuraPlayerController();

protected:
	// 重写BeginPlay函数，在控制器开始游戏时调用
	virtual void BeginPlay() override;

private:
	// 输入映射上下文，用于定义玩家输入与游戏操作的映射关系
	// UPROPERTY宏使该属性在编辑器中可编辑，并支持Unreal Engine的反射系统
	// EditAnywhere: 可以在蓝图编辑器中的任何地方（默认值、实例）编辑此属性
	// Category = "Input": 在编辑器属性面板中归入"Input"分类
	// TObjectPtr是UE5的智能指针类型，用于安全地引用UObject对象
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	// 通常这里还可以添加其他私有属性，例如：
	// - 输入动作引用（如移动、攻击、跳跃等）
	// - 控制相关的配置参数
	// - 内部状态变量
};

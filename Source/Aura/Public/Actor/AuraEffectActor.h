// Copyright Amor

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

// 前向声明USphereComponent类，避免包含头文件
// 前向声明是一种编译优化，只声明类名而不包含其完整定义
// 适用于在头文件中使用指针或引用类型时
class USphereComponent;

/**
 * Aura效果Actor类
 * 继承自AActor，用于创建游戏中的交互式效果物体（如药水、增益道具、陷阱等）
 *
 * 功能说明：
 * 1. 通过碰撞检测与玩家或敌人交互
 * 2. 应用游戏效果（如治疗、属性增益）
 * 3. 提供视觉表现和交互反馈
 *
 * 设计模式：
 * 使用组件化设计，将视觉、碰撞和逻辑分离
 */
UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
    // UE宏，生成反射和序列化所需的代码体
    GENERATED_BODY()

public:
    /**
     * 构造函数声明
     * 初始化效果Actor的组件和属性
     */
    AAuraEffectActor();

    /**
     * 开始重叠事件回调函数（虚拟函数，可被子类重写）
     * 当其他Actor进入碰撞范围时调用
     *
     * UFUNCTION()宏：使函数能被蓝图系统调用，并支持Unreal的反射系统
     *
     * @param OverlappedComp: 产生重叠事件的组件（通常是这个Actor的Sphere组件）
     * @param OtherActor: 进入碰撞范围的另一个Actor（通常是玩家或敌人）
     * @param OtherComp: 另一个Actor的碰撞组件
     * @param OtherBodyIndex: 其他组件的Body索引
     * @param bFromSweep: 是否来自扫描检测（连续碰撞检测）
     * @param SweepResult: 扫描检测的命中结果，包含位置、法线等信息
     *
     * 使用场景：
     * 1. 玩家拾取药水时应用治疗效果
     * 2. 玩家进入陷阱范围时应用伤害效果
     * 3. 触发机关或事件
     */
    UFUNCTION()
    virtual void OnOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    /**
     * 结束重叠事件回调函数（虚拟函数，可被子类重写）
     * 当其他Actor离开碰撞范围时调用
     *
     * @param OverlappedComp: 产生重叠事件的组件
     * @param OtherActor: 离开碰撞范围的另一个Actor
     * @param OtherComp: 另一个Actor的碰撞组件
     * @param OtherBodyIndex: 其他组件的Body索引
     *
     * 使用场景：
     * 1. 移除持续效果（如离开光环范围）
     * 2. 重置状态或触发后续事件
     * 3. 显示离开范围的提示
     */
    UFUNCTION()
    virtual void EndOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
    );

protected:
    /**
     * 重写父类的BeginPlay函数，在游戏开始时调用
     * 在这里进行Actor的运行时初始化
     *
     * 功能：
     * 1. 绑定碰撞事件委托
     * 2. 初始化视觉效果
     * 3. 设置初始状态
     */
    virtual void BeginPlay() override;

private:
    /**
     * 球体碰撞组件指针
     * UPROPERTY宏使其受到Unreal垃圾回收系统的管理
     *
     * 属性宏参数：
     * - VisibleAnywhere: 在编辑器中任何地方都可见（蓝图、详情面板），但不能编辑
     *   （如果需要编辑，应使用EditAnywhere）
     *
     * 功能说明：
     * 1. 用于检测与其他Actor的碰撞/重叠
     * 2. 定义效果的作用范围
     * 3. 可以调整半径来控制影响区域大小
     *
     * TObjectPtr是UE5的智能指针类型，用于安全地引用UObject对象
     */
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> Sphere;

    /**
     * 静态网格组件指针
     * 用于显示效果Actor的视觉模型
     *
     * 设计说明：
     * 1. 通常设置为根组件，决定Actor的位置和旋转
     * 2. 可以设置为各种形状（药水瓶、宝箱、魔法符文等）
     * 3. 可以在蓝图中指定具体的网格模型
     *
     */
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> Mesh;

};  
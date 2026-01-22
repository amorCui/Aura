// Copyright Amor

#include "Actor/AuraEffectActor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h" 

#include "Actor/AuraEffectActor.h"

/**
 * AAuraEffectActor 构造函数
 * 初始化效果Actor的组件和属性
 * 效果Actor是游戏世界中用于施加游戏效果（如药水、增益道具）的交互对象
 */
AAuraEffectActor::AAuraEffectActor()
{
    /**
     * 禁用Actor的Tick功能，优化性能
     * 效果Actor通常不需要每帧更新，只在特定事件（如碰撞）时触发
     */
    PrimaryActorTick.bCanEverTick = false;

    /**
     * 创建并设置静态网格组件作为视觉表现
     * CreateDefaultSubobject<>: 在构造函数中创建组件
     * "Mesh": 组件名称，在编辑器中标识
     * SetRootComponent: 将此组件设置为Actor的根组件
     * 根组件是所有其他组件的父级，决定了Actor的变换基础
     */
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    SetRootComponent(Mesh);

    /**
     * 创建并设置球体碰撞组件用于检测交互
     * USphereComponent: 球体形状的碰撞组件
     * "Sphere": 组件名称
     * Sphere->SetupAttachment: 将球体碰撞组件附加到根组件上
     * 球体碰撞用于检测玩家进入效果范围，触发重叠事件
     */
    Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
    Sphere->SetupAttachment(GetRootComponent());

}

/**
 * OnOverlap - 当其他Actor进入碰撞范围时调用
 * 这是球体碰撞组件的重叠开始事件回调函数
 *
 * @param OverlappedComp: 产生重叠事件的组件（通常是这个Actor的Sphere组件）
 * @param OtherActor: 进入碰撞范围的另一个Actor（通常是玩家或敌人）
 * @param OtherComp: 另一个Actor的碰撞组件
 * @param OtherBodyIndex: 其他组件的Body索引
 * @param bFromSweep: 是否来自扫描检测（连续碰撞检测）
 * @param SweepResult: 扫描检测的命中结果
 *
 * 功能说明：
 * 1. 检测进入范围的Actor是否拥有能力系统接口
 * 2. 获取其属性集并修改属性值
 * 3. 应用效果后销毁自身
 *
 */
void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    /**
     * TODO: 需要改为应用GameplayEffect，现在使用const_cast作为临时解决方案
     */
     // 检查OtherActor是否实现了IAbilitySystemInterface接口
     // 只有拥有能力系统的Actor（如玩家、敌人）才能接收效果
    if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
    {
        /**
         * 获取目标Actor的AuraAttributeSet
         * 步骤：
         * 1. 通过ASCInterface获取目标的能力系统组件
         * 2. 从能力系统组件中获取特定类型的属性集（UAuraAttributeSet）
         * 3. 使用StaticClass()获取UAuraAttributeSet的类信息
         */
        const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(
            ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass())
        );

        /**
         * 使用const_cast将const指针转换为非const指针
         * 这是临时的hack方法，用于绕过GAS的限制
         *
         */
        UAuraAttributeSet* MutableAuraAttributeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);

        /**
         * 修改目标的生命值属性（增加25点）
         * 直接调用属性集的Setter函数

         */
        MutableAuraAttributeSet->SetHealth(AuraAttributeSet->GetHealth() + 25.f);

        /**
         * 应用效果后销毁Actor
         * 模拟一次性的消耗品（如血瓶）
         * Destroy(): 标记Actor为待销毁，在下一帧清理
         */
        Destroy();
    }
}

/**
 * EndOverlap - 当其他Actor离开碰撞范围时调用
 * 球体碰撞组件的重叠结束事件回调函数
 *
 */
void AAuraEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

/**
 * BeginPlay - 游戏开始或Actor生成时调用
 * 在这里进行Actor的运行时初始化
 *
 * 功能说明：
 * 1. 调用父类BeginPlay确保基础初始化
 * 2. 绑定碰撞事件委托，使函数能够响应碰撞事件
 */
void AAuraEffectActor::BeginPlay()
{
    // 调用父类的BeginPlay，确保Actor正确初始化
    Super::BeginPlay();

    /**
     * 绑定球体碰撞组件的开始重叠事件委托
     * OnComponentBeginOverlap: 当其他组件开始与这个组件重叠时触发的事件
     * AddDynamic: 添加动态委托绑定
     *
     * 参数说明：
     * this: 拥有回调函数的对象
     * &AAuraEffectActor::OnOverlap: 回调函数指针
     *
     * 这行代码将OnOverlap函数与球体碰撞组件的开始重叠事件连接起来
     * 当玩家或敌人进入球体范围时，OnOverlap函数会被自动调用
     */
    Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);

}






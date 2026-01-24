// Copyright Amor


#include "UI/WidgetController/AuraWidgetController.h"

/**
 * 设置界面控制器参数函数
 * 将传入的界面控制器参数结构体中的值分别设置到对应的成员变量中
 *
 * @param WCParams 界面控制器参数结构体，包含初始化所需的所有游戏系统组件引用
 *
 * 函数功能说明：
 * 1. 这是一个简单的赋值函数，用于传递和存储界面控制器所需的关键对象引用
 * 2. 这些对象引用将被用于数据绑定、事件监听和UI更新
 * 3. 这是MVC模式中控制器(Model-View-Controller)与模型(Model)建立连接的关键步骤
 *
 * 设计模式：Setter注入模式
 * 通过参数注入依赖对象，而不是在控制器内部直接创建，提高了代码的模块化和可测试性
 */
void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
    /**
     * 赋值玩家控制器引用
     * 作用：
     * 1. 处理用户输入事件（如按键、鼠标操作）
     * 2. 控制摄像机视角
     * 3. 管理游戏状态和模式
     *
     * PlayerController是玩家与游戏世界交互的主要接口
     */
    PlayerController = WCParams.PlayerController;

    /**
     * 赋值玩家状态引用
     * 作用：
     * 1. 存储玩家数据（等级、经验、分数等）
     * 2. 在网络游戏中，PlayerState会在服务器和客户端之间复制
     * 3. 作为Gameplay Ability System(GAS)的拥有者，持有能力系统组件和属性集
     */
    PlayerState = WCParams.PlayerState;

    /**
     * 赋值能力系统组件引用
     * 作用：
     * 1. 管理角色的所有能力和效果
     * 2. 处理能力的激活、取消和冷却
     * 3. 应用GameplayEffects修改属性
     * 4. 提供属性变化的事件通知
     *
     * 这是GAS框架的核心组件，负责所有与能力相关的逻辑
     */
    AbilitySystemComponent = WCParams.AbilitySystemComponent;

    /**
     * 赋值属性集引用
     * 作用：
     * 1. 定义和存储角色的各种游戏属性（生命值、法力值、力量、敏捷等）
     * 2. 处理属性的基础值、当前值和修改器
     * 3. 提供属性访问和修改的接口
     *
     * 属性集是GAS框架中存储角色属性的容器
     */
    AttributeSet = WCParams.AttributeSet;

    /**
     * 函数执行后的状态说明：
     *
     * 设置完这些参数后，界面控制器拥有了：
     * 1. PlayerController: 用于响应输入和UI交互
     * 2. PlayerState: 用于获取玩家数据和状态
     * 3. AbilitySystemComponent: 用于监听能力变化和应用效果
     * 4. AttributeSet: 用于获取和监听属性变化
     *
     * 这使得控制器能够：
     * - 监听属性变化并更新UI
     * - 监听能力激活状态并更新技能图标
     * - 响应玩家输入并触发相应的能力
     * - 显示玩家状态信息
     */

    
}

void UAuraWidgetController::BroadCastInitialValues()
{
}



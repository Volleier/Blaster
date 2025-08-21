#pragma once

// 战斗状态枚举类型，供蓝图使用
UENUM(BlueprintType)
enum class ECombatState : uint8
{
	// 未占用状态
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),

	// 装弹状态
	ECS_Reloading UMETA(DisplayName = "Reloading"),

	// 投掷手雷状态
	ECS_ThrowingGrenade UMETA(DisplayName = "Throwing Grenade"),

	// 枚举最大值（用于默认或边界检查）
	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};
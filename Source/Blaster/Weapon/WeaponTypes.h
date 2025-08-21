#pragma once

// 定义射线长度常量，单位为浮点数
#define TRACE_LENGTH 80000.f

// 自定义深度常量，用于渲染相关的自定义效果
#define CUSTOM_DEPTH_PURPLE 250 // 紫色自定义深度
#define CUSTOM_DEPTH_BLUE 251	// 蓝色自定义深度
#define CUSTOM_DEPTH_TAN 252	// 棕色自定义深度

// 武器类型枚举，支持蓝图类型
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),		 // 突击步枪
	EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),	 // 火箭发射器
	EWT_Pistol UMETA(DisplayName = "Pistol"),					 // 手枪
	EWT_SubmachineGun UMETA(DisplayName = "Submachine Gun"),	 // 冲锋枪
	EWT_Shotgun UMETA(DisplayName = "Shotgun"),					 // 霰弹枪
	EWT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),		 // 狙击步枪
	EWT_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"), // 榴弹发射器

	EWT_MAX UMETA(DisplayName = "DefaultMAX") // 枚举最大值（默认）
};
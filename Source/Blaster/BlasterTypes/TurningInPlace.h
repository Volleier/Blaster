#pragma once

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_Left UMETA(DisplayName = "Turning Left"),		// 向左转
	ETIP_Right UMETA(DisplayName = "Turning Right"),	// 向右转
	ETIP_NotTurning UMETA(DisplayName = "Not Turning"), // 不转动

	ETIP_MAX UMETA(DisplayName = "DefaultMAX") // 默认最大值
};
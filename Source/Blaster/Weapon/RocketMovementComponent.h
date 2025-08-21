#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "RocketMovementComponent.generated.h"

/**
 * 火箭移动组件
 * 继承自 UProjectileMovementComponent，用于实现火箭类武器的移动逻辑。
 */
UCLASS()
class BLASTER_API URocketMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

protected:
	/**
	 * 处理火箭与阻挡物体碰撞的结果。
	 * @param Hit 碰撞结果信息
	 * @param TimeTick 当前时间片
	 * @param MoveDelta 移动的距离向量
	 * @param SubTickTimeRemaining 剩余的子时间片
	 * @return 返回碰撞处理结果
	 */
	virtual EHandleBlockingHitResult HandleBlockingHit(
		const FHitResult& Hit,
		float TimeTick,
		const FVector& MoveDelta,
		float& SubTickTimeRemaining
	) override;

	/**
	 * 处理火箭的碰撞冲击。
	 * @param Hit 碰撞结果信息
	 * @param TimeSlice 时间片（默认值为0）
	 * @param MoveDelta 移动的距离向量（默认值为零向量）
	 */
	virtual void HandleImpact(
		const FHitResult& Hit,
		float TimeSlice = 0.f,
		const FVector& MoveDelta = FVector::ZeroVector
	) override;
};
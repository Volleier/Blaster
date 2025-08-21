#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "ShieldPickup.generated.h"

/**
 * AShieldPickup 类用于表示护盾补给道具。
 * 继承自 APickup 基类，实现护盾补给的相关功能。
 */
UCLASS()
class BLASTER_API AShieldPickup : public APickup
{
	GENERATED_BODY()

protected:
	/**
	 * 当球形碰撞体发生重叠时调用的函数。
	 * @param OverlappedComponent 发生重叠的组件
	 * @param OtherActor 参与重叠的其他Actor
	 * @param OtherComp 参与重叠的其他组件
	 * @param OtherBodyIndex 其他组件的索引
	 * @param bFromSweep 是否为扫掠检测
	 * @param SweepResult 扫掠检测结果
	 */
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	/** 护盾补给的数值，默认为 100 */
	UPROPERTY(EditAnywhere)
	float ShieldReplenishAmount = 100.f;

	/** 护盾补给的持续时间，默认为 5 秒 */
	UPROPERTY(EditAnywhere)
	float ShieldReplenishTime = 5.f;
};

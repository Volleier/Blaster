#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "JumpPickup.generated.h"

/**
 * 跳跃增益拾取类
 * 继承自 APickup，用于实现角色拾取后获得跳跃能力增强的功能
 */
UCLASS()
class BLASTER_API AJumpPickup : public APickup
{
	GENERATED_BODY()

protected:
	/**
	 * 当球体碰撞体发生重叠时调用
	 * @param OverlappedComponent 发生重叠的组件
	 * @param OtherActor 与本组件重叠的其他Actor
	 * @param OtherComp 与本组件重叠的其他组件
	 * @param OtherBodyIndex 其他组件的Body索引
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
	/** 跳跃速度增益，单位为cm/s，默认值为4000 */
	UPROPERTY(EditAnywhere)
	float JumpZVelocityBuff = 4000.f;

	/** 跳跃增益持续时间，单位为秒，默认值为30 */
	UPROPERTY(EditAnywhere)
	float JumpBuffTime = 30.f;
};

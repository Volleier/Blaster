#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickup.generated.h"

/**
 * AHealthPickup 类
 * 继承自 APickup，用于实现血量拾取功能。
 */
UCLASS()
class BLASTER_API AHealthPickup : public APickup
{
	GENERATED_BODY()

public:
	// 构造函数
	AHealthPickup();

protected:
	/**
	 * 当球体碰撞体发生重叠时调用
	 * @param OverlappedComponent 发生重叠的组件
	 * @param OtherActor 参与重叠的其他Actor
	 * @param OtherComp 参与重叠的其他组件
	 * @param OtherBodyIndex 其他组件的Body索引
	 * @param bFromSweep 是否为Sweep产生的重叠
	 * @param SweepResult Sweep的结果
	 */
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	// 治疗量，拾取后恢复的血量
	UPROPERTY(EditAnywhere)
	float HealAmount = 100.f;

	// 治疗时间，拾取后持续治疗的时间
	UPROPERTY(EditAnywhere)
	float HealingTime = 5.f;
};
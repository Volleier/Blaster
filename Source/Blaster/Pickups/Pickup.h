#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

/**
 * APickup类用于表示游戏中的拾取物品。
 * 继承自AActor，包含拾取物品的相关组件和事件处理。
 */
UCLASS()
class BLASTER_API APickup : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数
	APickup();

	// 每帧调用的Tick函数
	virtual void Tick(float DeltaTime) override;

	// 当Actor被销毁时调用
	virtual void Destroyed() override;

protected:
	// 游戏开始时调用
	virtual void BeginPlay() override;

	/**
	 * 球体碰撞重叠事件处理函数
	 * @param OverlappedComponent 被重叠的组件
	 * @param OtherActor 参与重叠的其他Actor
	 * @param OtherComp 参与重叠的其他组件
	 * @param OtherBodyIndex 其他组件的Body索引
	 * @param bFromSweep 是否为Sweep产生的重叠
	 * @param SweepResult Sweep结果
	 */
	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// 旋转基础速率（度/秒），可在编辑器中设置
	UPROPERTY(EditAnywhere)
	float BaseTurnRate = 45.f;

private:
	// 球体碰撞组件，用于检测拾取物品的重叠
	UPROPERTY(EditAnywhere)
	class USphereComponent* OverlapSphere;

	// 拾取音效，拾取物品时播放
	UPROPERTY(EditAnywhere)
	class USoundCue* PickupSound;

	// 拾取物品的静态网格组件
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PickupMesh;

	// 拾取特效组件（Niagara），用于显示粒子效果
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* PickupEffectComponent;

	// 拾取特效系统（Niagara），用于生成特效
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;

    // 定时器句柄，用于延迟绑定重叠事件
    FTimerHandle BindOverlapTimer;
    // 绑定重叠事件的延迟时间（秒），默认0.25秒
    float BindOverlapTime = 0.25f;
    // 定时器回调函数，定时器结束后绑定重叠事件
    void BindOverlapTimerFinished();

public:
};

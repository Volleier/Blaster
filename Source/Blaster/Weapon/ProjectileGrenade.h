#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileGrenade.generated.h"

/**
 * AProjectileGrenade 类
 * 继承自 AProjectile，用于实现手雷类投射物的功能。
 */
UCLASS()
class BLASTER_API AProjectileGrenade : public AProjectile
{
	GENERATED_BODY()

public:
	// 构造函数
	AProjectileGrenade();

	// 当对象被销毁时调用
	virtual void Destroyed() override;

protected:
	// 游戏开始时调用
	virtual void BeginPlay() override;

	// 当手雷发生弹跳时调用
	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

private:
	// 弹跳时播放的音效
	UPROPERTY(EditAnywhere)
	USoundCue* BounceSound;
};
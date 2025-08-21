// 在项目设置的描述页面填写您的版权声明。

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

/**
 * 命中扫描武器类，继承自AWeapon
 */
UCLASS()
class BLASTER_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	// 重写武器开火方法，参数为命中目标位置
	virtual void Fire(const FVector& HitTarget) override;

protected:
	// 计算带有散射的射线终点
	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget);

	// 执行武器射线检测，返回命中结果
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit);

	// 命中粒子特效
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ImpactParticles;

	// 命中音效
	UPROPERTY(EditAnywhere)
	USoundCue* HitSound;

	// 武器伤害值
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

private:
	// 射线束粒子特效
	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;

	// 枪口闪光粒子特效
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	// 开火音效
	UPROPERTY(EditAnywhere)
	USoundCue* FireSound;

	/**
	 * 散射相关参数
	 */

	 // 散射球体距离
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float DistanceToSphere = 800.f;

	// 散射球体半径
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float SphereRadius = 75.f;

	// 是否启用散射
	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	bool bUseScatter = false;
};
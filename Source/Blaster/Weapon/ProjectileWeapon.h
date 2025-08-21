#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

UCLASS()
class BLASTER_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	// 重写父类的 Fire 方法，使用传入的目标位置进行射击
	virtual void Fire(const FVector& HitTarget) override;

private:
	// 可在编辑器中设置的属性，用于指定投射物的类
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;
};
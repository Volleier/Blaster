// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 50000.f

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();
	friend class ABlasterCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	// 装备武器
	void EquipWeapon(class AWeapon *WeaponToEquip);

protected:
	virtual void BeginPlay() override;
	// 设置瞄准状态
	void SetAiming(bool bIsAiming);

	// 服务器端设置瞄准状态
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	// 当装备的武器发生变化时调用
	UFUNCTION()
	void OnRep_EquippedWeapon();

	void FireButtonPressed(bool bIsPressed);

	UFUNCTION(Server,Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast,Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);
private:
	// 角色指针
	class ABlasterCharacter* Character;
	class ABlasterPlayerController* Controller;
	class ABlasterHUD* HUD;

	// 装备的武器，使用OnRep_EquippedWeapon进行同步
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon *EquippedWeapon;

	// 是否在瞄准状态
	UPROPERTY(Replicated)
	bool bAiming;

	// 基础行走速度
	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	// 瞄准时的行走速度
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	// 是否按下开火键
	bool bFireButtonPressed;

	// 设置准星的HUDPackage
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	FVector HitTarget;

	/*
	 * 瞄准和FOV
	 */
    // 未瞄准时的视野范围；在BeginPlay中设置为相机的基础视野范围
	float DefaultFOV;

	UPROPERTY(EditAnywhere,Category = Combat)	
	float ZoomedFOV = 30.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterSpeed = 20.f;

	void InterFOV(float DeltaTime);

public:
};
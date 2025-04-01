// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Components/ActorComponent.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "Blaster/BlasterTypes/CombatState.h"
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
	void Reload();

	// 结束装填
	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	// 处理开火按钮按下事件
	void FireButtonPressed(bool bIsPressed);

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

	// 执行开火操作
	void Fire();

	// 服务器端处理开火请求
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize &TraceHitTarget);

	// 多播开火事件
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize &TraceHitTarget);

	// 在准星下进行射线检测
	void TraceUnderCrosshairs(FHitResult &TraceHitResult);

	// 设置HUD准星
	void SetHUDCrosshairs(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void ServerReload();

	void HandleReload();

	int32 AmountToReload();

private:
	// 角色指针
	UPROPERTY()
	class ABlasterCharacter *Character;
	UPROPERTY()
	class ABlasterPlayerController *Controller;
	UPROPERTY()
	class ABlasterHUD *HUD;

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

	// 击中目标的位置
	FVector HitTarget;

	// HUD包，用于设置准星
	FHUDPackage HUDPackage;

	/*
	 * 瞄准和FOV
	 */
	// 未瞄准时的视野范围；在BeginPlay中设置为相机的基础视野范围
	float DefaultFOV;

	// 瞄准时的视野范围
	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV = 30.f;

	// 当前的视野范围
	float CurrentFOV;

	// 瞄准时视野变化的插值速度
	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterSpeed = 20.f;

	// 插值更新视野范围
	void InterFOV(float DeltaTime);

	/*
	 * 自动开火
	 */
	// 开火计时器句柄
	FTimerHandle FireTimer;
	// 是否可以开火
	bool bCanFire = true;

	// 开始开火计时器
	void StartFireTimer();
	// 开火计时器结束
	void FireTimerFinished();

	bool CanFire();

	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int StartingAmmo = 30;

	UPROPERTY(EditAnywhere)
	int32 StartingRocketAmmo = 0;

	void InitializeCarriedAmmo();

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
	void OnRep_CombatState();

	void UpdateAmmoValues();
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "Components/TimelineComponent.h"
#include "BlasterCharacter.generated.h"

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	void PlayFireMontage(bool bAiming);
	void PlayElimMontage();

	// 多播函数，用于处理命中效果
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHit();

	virtual void OnRep_ReplicatedMovement() override;

	void Elim();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();

	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	// 移动前进
	void MoveForward(float Value);
	// 移动右侧
	void MoveRight(float Value);
	// 转向
	void Turn(float Value);
	// 向上看
	void LookUp(float Value);
	// 装备按钮按下
	void EquipButtonPressed();
	// 蹲下按钮按下
	void CrouchButtonPressed();
	// 瞄准按钮按下
	void AimButtonPressed();
	// 瞄准按钮释放
	void AimButtonReleased();
	// 瞄准偏移
	void AimOffset(float DeltaTime);
	virtual void Jump() override;
	// 开火按钮按下
	void FireButtonPressed();
	// 开火按钮释放
	void FireButtonReleased();
	// 播放命中反应动画
	void PlayHitReactMontage();
	// 计算瞄准偏移角度
	void CalculateAO_Pitch();
	// 代理转向
	void SimProxiesTurn();

	// 角色接受伤害
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                   class AController* InstigatorController, AActor* DamageCauser);
	void UpdateHUDHealth();
	// 轮询任何相关类并初始化我们的HUD
	void PollInit();

private:
	// 摄像机臂组件
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	// 跟随摄像机组件
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	// 头顶小部件
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	// 重叠的武器
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	// 重叠武器的回调函数
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	// 战斗组件
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;

	// 服务器端装备按钮按下
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	// 瞄准偏移的偏航角度
	float AO_Yaw;
	// 插值后的瞄准偏移偏航角度
	float InterpAO_Yaw;
	// 瞄准偏移的俯仰角度
	float AO_Pitch;
	// 初始瞄准旋转
	FRotator StartingAimRotation;

	// 转向状态
	ETurningInPlace TurningInPlace;
	// 在原地转向
	void TurnInPlace(float DeltaTime);

	// 开火武器动画蒙太奇
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UAnimMontage* FireWeaponMontage;

	// 命中反应动画蒙太奇
	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* HitReactMontage;

	// 消除动画蒙太奇
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;

	// 如果角色靠近则隐藏摄像机
	void HideCameraIfCharacterClose();
	// 摄像机阈值
	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	// 是否旋转根骨骼
	bool bRotateRootBone;
	// 转向阈值
	float TurnThreshold = 0.5f;
	// 代理上次帧的旋转
	FRotator ProxyRotationLastFrame;
	// 代理当前帧的旋转
	FRotator ProxyRotation;
	// 代理的偏航角度
	float ProxyYaw;
	// 自上次移动复制以来的时间
	float TimeSinceLastMovementReplication;
	// 计算速度
	float CalculateSpeed();

	// 最大生命值
	UPROPERTY(EditAnywhere, Category = "Player States")
	float MaxHealth = 100.f;
	// 当前生命值
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player States")
	float Health = 100.f;

	// 生命值变化的回调函数
	UFUNCTION()
	void OnRep_Health();

	// 角色控制器
	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

	bool bElimmed = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

	void ElimTimerFinished();

	/*
	 * 溶解效果
	 */
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;
	FOnTimelineFloat DissolveTrack;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();

	// 动态溶解材质实例
	UPROPERTY(VisibleAnywhere, Category = Elim)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// 蓝图中设置的溶解材质实例
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* DissolveMaterialInstance;

	/*
	* 消除效果
	*/
	UPROPERTY(EditAnywhere)
	UParticleSystem* ElimBotEffect;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ElimBotComponent;

	UPROPERTY(EditAnywhere)
	class USoundCue* ElimBotSound;

	UPROPERTY()
	class ABlasterPlayerState* BlasterPlayerState;
public:
	// 设置重叠的武器
	void SetOverlappingWeapon(AWeapon* Weapon);
	// 是否装备了武器
	bool IsWeaponEquipped();
	// 是否在瞄准
	bool IsAiming();
	// 获取瞄准偏移的偏航角度
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	// 获取瞄准偏移的俯仰角度
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	// 获取装备的武器
	AWeapon* GetEquippedWeapon();
	// 获取转向状态
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	// 获取命中目标
	FVector GetHitTarget() const;

	// 获取跟随摄像机
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	// 是否应该旋转根骨骼
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	// 是否被消除
	FORCEINLINE bool IsElimmed() const { return bElimmed; }

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
};

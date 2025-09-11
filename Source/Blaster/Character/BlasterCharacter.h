// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "Components/TimelineComponent.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "BlasterCharacter.generated.h"

/**
 * ABlasterCharacter
 * ��Ҫ��ɫ�࣬�̳���ACharacter���Զ���ӿ�IInteractWithCrosshairsInterface
 * ������ɫ���ƶ���ս���������������ȹ���
 */
UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();

	// ÿ֡����
	virtual void Tick(float DeltaTime) override;

	// ��������������
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �������Ը���
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// �����ʼ�������
	virtual void PostInitializeComponents() override;

	// ���ſ��𶯻���̫��
	void PlayFireMontage(bool bAiming);

	// ���Ż���������̫��
	void PlayReloadMontage();

	// ��������������̫��
	void PlayElimMontage();

	// ����Ͷ�����񵯶�����̫��
	void PlayThrowGrenadeMontage();

	// �ಥ����Ч��
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHit();

	// �����ƶ�ʱ����
	virtual void OnRep_ReplicatedMovement() override;

	// ��ɫ����
	void Elim();

	// �ಥ����Ч��
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();

	// ����ʱ����
	virtual void Destroyed() override;

	// �Ƿ������Ϸ�淨
	UPROPERTY(Replicated)
	bool bDisableGameplay = false;

	// ��ͼʵ�֣���ʾ/���ؾѻ���UI
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

	// ����HUD����ֵ
	void UpdateHUDHealth();

	// ����HUD����
	void UpdateHUDShield();

	// ����HUD��ҩ
	void UpdateHUDAmmo();

	// ����Ĭ������
	void SpawnDefaultWeapon();

protected:
	// ��Ϸ��ʼʱ����
	virtual void BeginPlay() override;

	// �ƶ�ǰ��
	void MoveForward(float Value);

	// �ƶ��Ҳ�
	void MoveRight(float Value);

	// ת��
	void Turn(float Value);

	// ���Ͽ�
	void LookUp(float Value);

	// װ����ť����
	void EquipButtonPressed();

	// ���°�ť����
	void CrouchButtonPressed();

	// ��׼��ť����
	void AimButtonPressed();

	// ��׼��ť�ͷ�
	void AimButtonReleased();

	// ��׼ƫ��
	void AimOffset(float DeltaTime);

	// ��Ծ
	virtual void Jump() override;

	// ����ť����
	void FireButtonPressed();

	// ����ť�ͷ�
	void FireButtonReleased();

	// ������ť����
	void ReloadButtonPressed();

	// �������з�Ӧ����
	void PlayHitReactMontage();

	// ������׼ƫ�ƽǶ�
	void CalculateAO_Pitch();

	// ����ת��
	void SimProxiesTurn();

	// Ͷ�����񵯰�ť����
	void GrenadeButtonPressed();

	// ��������������
	void DropOrDestroyWeapon(AWeapon* Weapon);
	void DropOrDestroyWeapons();

	// ��ɫ�����˺�
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		class AController* InstigatorController, AActor* DamageCauser);

	// ��ѯ����ಢ��ʼ��HUD
	void PollInit();

	// ԭ����ת
	void RotateInPlace(float DeltaTime);

private:
	// ����������
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	// ������������
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	// ͷ��С����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	// �ص�������
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	// �ص������ص�
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	// ս�����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* Combat;

	// BUFF���
	UPROPERTY(VisibleAnywhere)
	class UBuffComponent* Buff;

	// ��������װ����ť����
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	// ��׼ƫ�Ƶ�ƫ���Ƕ�
	float AO_Yaw;

	// ��ֵ�����׼ƫ��ƫ���Ƕ�
	float InterpAO_Yaw;

	// ��׼ƫ�Ƶĸ����Ƕ�
	float AO_Pitch;

	// ��ʼ��׼��ת
	FRotator StartingAimRotation;

	// ת��״̬
	ETurningInPlace TurningInPlace;

	// ԭ��ת��
	void TurnInPlace(float DeltaTime);

	// ��������������̫��
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UAnimMontage* FireWeaponMontage;

	// ����������̫��
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;

	// ���з�Ӧ������̫��
	UPROPERTY(EditAnywhere, Category = "Combat")
	UAnimMontage* HitReactMontage;

	// ����������̫��
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ElimMontage;

	// Ͷ�����񵯶�����̫��
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ThrowGrenadeMontage;

	// ������ɫʱ���������
	void HideCameraIfCharacterClose();

	// �����������ֵ
	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	// �Ƿ���ת������
	bool bRotateRootBone;

	// ת����ֵ
	float TurnThreshold = 0.5f;

	// �����ϴ�֡����ת
	FRotator ProxyRotationLastFrame;

	// ����ǰ֡����ת
	FRotator ProxyRotation;

	// �����ƫ���Ƕ�
	float ProxyYaw;

	// �����ϴ��ƶ����Ƶ�ʱ��
	float TimeSinceLastMovementReplication;

	// �����ٶ�
	float CalculateSpeed();

	// �������ֵ
	UPROPERTY(EditAnywhere, Category = "Player States")
	float MaxHealth = 100.f;

	// ��ǰ����ֵ
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player States")
	float Health = 100.f;

	// ����ֵ�仯�ص�
	UFUNCTION()
	void OnRep_Health(float LastHealth);

	// ��󻤶�ֵ
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxShield = 100.f;

	// ��ǰ����ֵ
	UPROPERTY(ReplicatedUsing = OnRep_Shield, EditAnywhere, Category = "Player Stats")
	float Shield = 0.f;

	// ���ܱ仯�ص�
	UFUNCTION()
	void OnRep_Shield(float LastShield);

	// ��ɫ������
	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

	// �Ƿ�������
	bool bElimmed = false;

	// ������ʱ��
	FTimerHandle ElimTimer;

	// �����ӳ�
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

	// ������ʱ������
	void ElimTimerFinished();

	/*
	 * �ܽ�Ч�����
	 */
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;

	// �ܽ�켣
	FOnTimelineFloat DissolveTrack;

	// �ܽ�����
	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;

	// �ܽ���ʸ���
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);

	// ��ʼ�ܽ�
	void StartDissolve();

	// ��̬�ܽ����ʵ��
	UPROPERTY(VisibleAnywhere, Category = Elim)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// ��ͼ���õ��ܽ����ʵ��
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* DissolveMaterialInstance;

	/*
	 * ������Ч���
	 */
	UPROPERTY(EditAnywhere)
	UParticleSystem* ElimBotEffect;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ElimBotComponent;

	UPROPERTY(EditAnywhere)
	class USoundCue* ElimBotSound;

	UPROPERTY()
	class ABlasterPlayerState* BlasterPlayerState;

	/**
	 * �������
	 */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AttachedGrenade;


	/**
	* Ĭ������
	*/
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> DefaultWeaponClass;

public:
	// �����ص�����
	void SetOverlappingWeapon(AWeapon* Weapon);

	// �Ƿ�װ������
	bool IsWeaponEquipped();

	// �Ƿ�����׼
	bool IsAiming();

	// ��ȡ��׼ƫ�Ƶ�ƫ���Ƕ�
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }

	// ��ȡ��׼ƫ�Ƶĸ����Ƕ�
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }

	// ��ȡװ��������
	AWeapon* GetEquippedWeapon();

	// ��ȡת��״̬
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }

	// ��ȡ����Ŀ��
	FVector GetHitTarget() const;

	// ��ȡ���������
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// �Ƿ�Ӧ����ת������
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }

	// �Ƿ�������
	FORCEINLINE bool IsElimmed() const { return bElimmed; }

	// ��ȡ����ֵ
	FORCEINLINE float GetHealth() const { return Health; }

	// ��������ֵ
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }

	// ��ȡ�������ֵ
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	// ��ȡ����ֵ
	FORCEINLINE float GetShield() const { return Shield; }

	// ���û���ֵ
	FORCEINLINE void SetShield(float Amount) { Shield = Amount; }

	// ��ȡ��󻤶�ֵ
	FORCEINLINE float GetMaxShield() const { return MaxShield; }

	// ��ȡս�����
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }

	// ��ȡ������Ϸ�淨״̬
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }

	// ��ȡ����������̫��
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }

	// ��ȡ���ӵ��������
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }

	// ��ȡս��״̬
	ECombatState GetCombatState() const;

	// ��ȡBUFF���
	FORCEINLINE UBuffComponent* GetBuff() const { return Buff; }

	// �����Ƿ񱾵�װ��
	bool IsLocallyReloading();
};

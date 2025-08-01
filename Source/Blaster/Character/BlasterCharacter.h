// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "Components/TimelineComponent.h"
#include "Blaster/BlasterTypes/CombatState.h"
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
	void PlayReloadMontage();
	void PlayElimMontage();
	void PlayThrowGrenadeMontage();

	// �ಥ���������ڴ�������Ч��
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHit();

	virtual void OnRep_ReplicatedMovement() override;

	void Elim();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();

	virtual void Destroyed() override;

	UPROPERTY(Replicated)
	bool bDisableGameplay = false;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

protected:
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
	// ת��ť����
	void GrenadeButtonPressed();

	// ��ɫ�����˺�
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		class AController* InstigatorController, AActor* DamageCauser);
	void UpdateHUDHealth();
	// ��ѯ�κ�����ಢ��ʼ�����ǵ�HUD
	void PollInit();

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

	// �ص������Ļص�����
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	// ս�����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* Combat;

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
	// ��ԭ��ת��
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

	// �����ɫ���������������
	void HideCameraIfCharacterClose();
	// �������ֵ
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
	// ���ϴ��ƶ�����������ʱ��
	float TimeSinceLastMovementReplication;
	// �����ٶ�
	float CalculateSpeed();

	// �������ֵ
	UPROPERTY(EditAnywhere, Category = "Player States")
	float MaxHealth = 100.f;
	// ��ǰ����ֵ
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player States")
	float Health = 100.f;

	// ����ֵ�仯�Ļص�����
	UFUNCTION()
	void OnRep_Health();

	// ��ɫ������
	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

	bool bElimmed = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

	void ElimTimerFinished();

	/*
	 * �ܽ�Ч��
	 */
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeline;
	FOnTimelineFloat DissolveTrack;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();

	// ��̬�ܽ����ʵ��
	UPROPERTY(VisibleAnywhere, Category = Elim)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// ��ͼ�����õ��ܽ����ʵ��
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* DissolveMaterialInstance;

	/*
	* ����Ч��
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
	* Grenade
	*/

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AttachedGrenade;

public:
	// �����ص�������
	void SetOverlappingWeapon(AWeapon* Weapon);
	// �Ƿ�װ��������
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
	// �Ƿ�����
	FORCEINLINE bool IsElimmed() const { return bElimmed; }

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }

	ECombatState GetCombatState() const;
};

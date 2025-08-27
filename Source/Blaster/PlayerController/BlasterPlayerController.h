// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

/**
 * ABlasterPlayerController
 * ��ҿ������࣬�������HUD��ʾ�����״̬ͬ��������״̬����ȹ��ܡ�
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	/**
	 * ����HUD�ϵ�����ֵ��ʾ
	 * @param Health ��ǰ����ֵ
	 * @param MaxHealth �������ֵ
	 */
	void SetHUDHealth(float Health, float MaxHealth);

	/**
	 * ����HUD�ϵĻ���ֵ��ʾ
	 * @param Shield ��ǰ����ֵ
	 * @param MaxShield ��󻤶�ֵ
	 */
	void SetHUDShield(float Shield, float MaxShield);

	/**
	 * ����HUD�ϵķ�����ʾ
	 * @param Score ��ǰ����
	 */
	void SetHUDScore(float Score);

	/**
	 * ����HUD�ϵĻ�������ʾ
	 * @param Defeats ��ǰ������
	 */
	void SetHUDDefeats(int32 Defeats);

	/**
	 * ����HUD�ϵ�������ҩ��ʾ
	 * @param Ammo ��ǰ������ҩ��
	 */
	void SetHUDWeaponAmmo(int32 Ammo);

	/**
	 * ����HUD�ϵ�Я����ҩ��ʾ
	 * @param Ammo ��ǰЯ����ҩ��
	 */
	void SetHUDCarriedAmmo(int32 Ammo);

	/**
	 * ����HUD�ϵı�������ʱ��ʾ
	 * @param CountdownTime ����ʱʱ�䣨�룩
	 */
	void SetHUDMatchCountdown(float CountdownTime);

	/**
	 * ����HUD�ϵĹ��浹��ʱ��ʾ
	 * @param CountdownTime ���浹��ʱʱ�䣨�룩
	 */
	void SetHUDAnnouncementCountdown(float CountdownTime);

	/**
	 * ����HUD�ϵ�����������ʾ
	 * @param Grenades ��ǰ��������
	 */
	void SetHUDGrenades(int32 Grenades);

	/**
	 * ����ҿ�����ӵ��һ��Pawnʱ����
	 * @param InPawn �����Ƶ�Pawn
	 */
	virtual void OnPossess(APawn* InPawn) override;

	/**
	 * ÿ֡����Tick
	 * @param DeltaTime ������һ֡��ʱ�䣨�룩
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * ��ȡ������ʱ�䣬����ʱ��ͬ��
	 * @return ������ʱ�䣨�룩
	 */
	virtual float GetServerTime();

	/**
	 * ��ҽ���ʱ����ͬ��ʱ��
	 */
	virtual void ReceivedPlayer() override;

	/**
	 * ��ȡ��Ҫ�����������ڸ��Ƶ�����
	 * @param OutLifetimeProps ��Ҫ���Ƶ���������
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * ����״̬����ʱ�Ļص�
	 * @param State ����״̬
	 */
	void OnMatchStateSet(FName State);

	/**
	 * ���������ʼ�߼�
	 */
	void HandleMatchHasStarted();

	/**
	 * ���������ȴ�׶��߼�
	 */
	void HandleCooldown();

protected:
	/**
	 * ��Ϸ��ʼʱ����
	 */
	virtual void BeginPlay() override;

	/**
	 * ����HUD�ϵ�ʱ����ʾ
	 */
	void SetHUDTime();

	/**
	 * ��ѯ��ʼ��HUD
	 */
	void PollInit();

	/**
	 * �ڿͻ��˺ͷ�����֮��ͬ��ʱ��
	 */

	 /**
	  * �����������ǰʱ�䣬�����Ϳͻ�������ʱ��
	  * @param TimeOfClientRequest �ͻ�������ʱ��
	  */
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	/**
	 * ��������Ӧ�ͻ���ʱ�����󣬴��ͷ������յ������ʱ��
	 * @param TimeOfClientRequest �ͻ�������ʱ��
	 * @param TimeServerReceivedClientRequest �������յ������ʱ��
	 */
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	/** �ͻ����������ʱ��� */
	float ClientServerDelta = 0.f;

	/** ʱ��ͬ��Ƶ�ʣ��룩 */
	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	/** ʱ��ͬ������ʱ�� */
	float TimeSyncRunningTime = 0.f;

	/**
	 * ����Ƿ���Ҫ����ʱ��ͬ��
	 * @param DeltaTime ������һ֡��ʱ�䣨�룩
	 */
	void CheckTimeSync(float DeltaTime);

	/**
	 * �������������״̬
	 */
	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();

	/**
	 * �ͻ��˼�����;����ʱ�Ļص�
	 * @param StateOfMatch ��ǰ����״̬
	 * @param Warmup ����ʱ��
	 * @param Match ����ʱ��
	 * @param Cooldown ��ȴʱ��
	 * @param StartingTime ������ʼʱ��
	 */
	UFUNCTION(Client, Reliable)
	void ClientJoinMidgame(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime);

	/**
	* ���ӳپ��洦����
	* ����⵽��������ӳٹ���ʱ������HUD������ʾ������߼�
	*/
	void HighPingWarning();

	/**
	* ֹͣ���ӳپ��洦����
	* ����������ӳٻָ�����ʱ���ر�HUD������ʾ������߼�
	*/
	void StopHighPingWarning();

	/**
	* �����������ӳ�
	* @param DeltaTime ������һ֡��ʱ�䣨�룩
	* ���ڼ����ҵ�Pingֵ���ж��Ƿ���Ҫ�������ӳپ���
	*/
	void CheckPing(float DeltaTime);

private:
	/** HUD����ָ�� */
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;

	/** ��Ϸģʽ����ָ�� */
	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;

	/** �ؿ���ʼʱ�� */
	float LevelStartingTime = 0.f;

	/** ����ʱ�� */
	float MatchTime = 0.f;

	/** ����ʱ�� */
	float WarmupTime = 0.f;

	/** ��ȴʱ�� */
	float CooldownTime = 0.f;

	/** ����ʱ����ֵ */
	uint32 CountdownInt = 0;

	/** ����״̬��ʹ��OnRep_MatchState���и���֪ͨ */
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	/**
	 * ����״̬����֪ͨ�ص�
	 */
	UFUNCTION()
	void OnRep_MatchState();

	/** ��ɫHUD���Ӳ�ָ�� */
	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	/** HUD����ֵ */
	float HUDHealth;

	/** �Ƿ��ʼ������ֵ */
	bool bInitializeHealth = false;

	/** HUD�������ֵ */
	float HUDMaxHealth;

	/** HUD���� */
	float HUDScore;

	/** �Ƿ��ʼ������ */
	bool bInitializeScore = false;

	/** HUD������ */
	int32 HUDDefeats;

	/** �Ƿ��ʼ�������� */
	bool bInitializeDefeats = false;

	/** HUD�������� */
	int32 HUDGrenades;

	/** �Ƿ��ʼ���������� */
	bool bInitializeGrenades = false;

	/** HUD����ֵ */
	float HUDShield;

	/** �Ƿ��ʼ������ֵ */
	bool bInitializeShield = false;

	/** HUD��󻤶�ֵ */
	float HUDMaxShield;

	/** HUDЯ����ҩ */
	float HUDCarriedAmmo;

	/** �Ƿ��ʼ��Я����ҩ */
	bool bInitializeCarriedAmmo = false;

	/** HUD������ҩ */
	float HUDWeaponAmmo;

	/** �Ƿ��ʼ��������ҩ */
	bool bInitializeWeaponAmmo = false;

	/** ���ӳټ�ʱ����ʱ�����룩 */
	float HighPingRunningTime = 0.f;

	/** ���ӳپ������ʱ�䣨�룩�����ڱ༭�������� */
	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5.f;

	/** Ping��������ʱ�����룩 */
	float PingAnimationRunningTime = 0.f;

	/** ���Ping��Ƶ�ʣ��룩�����ڱ༭�������� */
	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 20.f;

	/** ���ӳ���ֵ�����룩�����ڱ༭�������� */
	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 50.f;
};
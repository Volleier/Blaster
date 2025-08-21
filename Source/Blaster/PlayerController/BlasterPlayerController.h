// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

/**
 * ABlasterPlayerController
 * 玩家控制器类，负责管理HUD显示、玩家状态同步、比赛状态处理等功能。
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	/**
	 * 设置HUD上的生命值显示
	 * @param Health 当前生命值
	 * @param MaxHealth 最大生命值
	 */
	void SetHUDHealth(float Health, float MaxHealth);

	/**
	 * 设置HUD上的护盾值显示
	 * @param Shield 当前护盾值
	 * @param MaxShield 最大护盾值
	 */
	void SetHUDShield(float Shield, float MaxShield);

	/**
	 * 设置HUD上的分数显示
	 * @param Score 当前分数
	 */
	void SetHUDScore(float Score);

	/**
	 * 设置HUD上的击败数显示
	 * @param Defeats 当前击败数
	 */
	void SetHUDDefeats(int32 Defeats);

	/**
	 * 设置HUD上的武器弹药显示
	 * @param Ammo 当前武器弹药数
	 */
	void SetHUDWeaponAmmo(int32 Ammo);

	/**
	 * 设置HUD上的携带弹药显示
	 * @param Ammo 当前携带弹药数
	 */
	void SetHUDCarriedAmmo(int32 Ammo);

	/**
	 * 设置HUD上的比赛倒计时显示
	 * @param CountdownTime 倒计时时间（秒）
	 */
	void SetHUDMatchCountdown(float CountdownTime);

	/**
	 * 设置HUD上的公告倒计时显示
	 * @param CountdownTime 公告倒计时时间（秒）
	 */
	void SetHUDAnnouncementCountdown(float CountdownTime);

	/**
	 * 设置HUD上的手雷数量显示
	 * @param Grenades 当前手雷数量
	 */
	void SetHUDGrenades(int32 Grenades);

	/**
	 * 当玩家控制器拥有一个Pawn时调用
	 * @param InPawn 被控制的Pawn
	 */
	virtual void OnPossess(APawn* InPawn) override;

	/**
	 * 每帧调用Tick
	 * @param DeltaTime 距离上一帧的时间（秒）
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * 获取服务器时间，用于时间同步
	 * @return 服务器时间（秒）
	 */
	virtual float GetServerTime();

	/**
	 * 玩家接收时立即同步时间
	 */
	virtual void ReceivedPlayer() override;

	/**
	 * 获取需要进行生命周期复制的属性
	 * @param OutLifetimeProps 需要复制的属性数组
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * 比赛状态设置时的回调
	 * @param State 比赛状态
	 */
	void OnMatchStateSet(FName State);

	/**
	 * 处理比赛开始逻辑
	 */
	void HandleMatchHasStarted();

	/**
	 * 处理比赛冷却阶段逻辑
	 */
	void HandleCooldown();

protected:
	/**
	 * 游戏开始时调用
	 */
	virtual void BeginPlay() override;

	/**
	 * 设置HUD上的时间显示
	 */
	void SetHUDTime();

	/**
	 * 轮询初始化HUD
	 */
	void PollInit();

	/**
	 * 在客户端和服务器之间同步时间
	 */

	 /**
	  * 向服务器请求当前时间，并传送客户端请求时间
	  * @param TimeOfClientRequest 客户端请求时间
	  */
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	/**
	 * 服务器响应客户端时间请求，传送服务器收到请求的时间
	 * @param TimeOfClientRequest 客户端请求时间
	 * @param TimeServerReceivedClientRequest 服务器收到请求的时间
	 */
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	/** 客户端与服务器时间差 */
	float ClientServerDelta = 0.f;

	/** 时间同步频率（秒） */
	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	/** 时间同步运行时长 */
	float TimeSyncRunningTime = 0.f;

	/**
	 * 检查是否需要进行时间同步
	 * @param DeltaTime 距离上一帧的时间（秒）
	 */
	void CheckTimeSync(float DeltaTime);

	/**
	 * 向服务器检查比赛状态
	 */
	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();

	/**
	 * 客户端加入中途比赛时的回调
	 * @param StateOfMatch 当前比赛状态
	 * @param Warmup 热身时间
	 * @param Match 比赛时间
	 * @param Cooldown 冷却时间
	 * @param StartingTime 比赛开始时间
	 */
	UFUNCTION(Client, Reliable)
	void ClientJoinMidgame(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime);

private:
	/** HUD对象指针 */
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;

	/** 游戏模式对象指针 */
	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;

	/** 关卡开始时间 */
	float LevelStartingTime = 0.f;

	/** 比赛时间 */
	float MatchTime = 0.f;

	/** 热身时间 */
	float WarmupTime = 0.f;

	/** 冷却时间 */
	float CooldownTime = 0.f;

	/** 倒计时整数值 */
	uint32 CountdownInt = 0;

	/** 比赛状态，使用OnRep_MatchState进行复制通知 */
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	/**
	 * 比赛状态复制通知回调
	 */
	UFUNCTION()
	void OnRep_MatchState();

	/** 角色HUD叠加层指针 */
	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	/** HUD生命值 */
	float HUDHealth;

	/** 是否初始化生命值 */
	bool bInitializeHealth = false;

	/** HUD最大生命值 */
	float HUDMaxHealth;

	/** HUD分数 */
	float HUDScore;

	/** 是否初始化分数 */
	bool bInitializeScore = false;

	/** HUD击败数 */
	int32 HUDDefeats;

	/** 是否初始化击败数 */
	bool bInitializeDefeats = false;

	/** HUD手雷数量 */
	int32 HUDGrenades;

	/** 是否初始化手雷数量 */
	bool bInitializeGrenades = false;

	/** HUD护盾值 */
	float HUDShield;

	/** 是否初始化护盾值 */
	bool bInitializeShield = false;

	/** HUD最大护盾值 */
	float HUDMaxShield;
};
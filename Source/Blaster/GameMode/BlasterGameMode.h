// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

// 命名空间用于定义比赛状态相关的常量
namespace MatchState
{
	// 比赛已达到持续时间，进入冷却阶段。显示获胜者并开始冷却计时器。
	extern BLASTER_API const FName Cooldown;
}

/**
 * ABlasterGameMode
 * 游戏模式类，负责管理比赛流程、玩家淘汰与重生、计时等功能。
 */
UCLASS()
class BLASTER_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// 构造函数
	ABlasterGameMode();

	// 每帧调用的Tick函数
	virtual void Tick(float DeltaTime) override;

	/**
	 * 玩家被淘汰时调用
	 * @param ElimmedCharacter 被淘汰的角色
	 * @param VictimController 被淘汰玩家的控制器
	 * @param AttackerController 攻击者的控制器
	 */
	virtual void PlayerEliminated(
		class ABlasterCharacter* ElimmedCharacter,
		class ABlasterPlayerController* VictimController,
		ABlasterPlayerController* AttackerController);

	/**
	 * 请求玩家重生
	 * @param ElimmedCharacter 被淘汰的角色
	 * @param ElimmedController 被淘汰角色的控制器
	 */
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);

	// 比赛准备阶段时间（秒）
	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.f;

	// 比赛正式阶段时间（秒）
	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;

	// 比赛冷却阶段时间（秒）
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.f;

	// 关卡开始时间
	float LevelStartingTime = 0.f;

protected:
	// 游戏开始时调用
	virtual void BeginPlay() override;

	// 比赛状态设置时调用
	virtual void OnMatchStateSet() override;

private:
	// 当前倒计时时间（秒）
	float CountdownTime = 0.f;

public:
	// 获取当前倒计时时间
	FORCEINLINE float GetCountdownTime() const { return CountdownTime; }
};
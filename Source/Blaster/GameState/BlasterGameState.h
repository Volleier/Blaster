// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BlasterGameState.generated.h"

/**
 * ABlasterGameState
 *
 * 游戏状态类，继承自 AGameState。
 * 用于管理游戏中的最高分和得分玩家列表。
 */
UCLASS()
class BLASTER_API ABlasterGameState : public AGameState
{
	GENERATED_BODY()

public:
	/**
	 * 获取需要进行网络同步的属性列表。
	 * @param OutLifetimeProps 用于存储需要同步的属性。
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * 更新最高分玩家列表。
	 * @param ScoringPlayer 当前得分的玩家。
	 */
	void UpdateTopScore(class ABlasterPlayerState* ScoringPlayer);

	/**
	 * 最高分玩家列表，支持网络同步。
	 */
	UPROPERTY(Replicated)
	TArray<class ABlasterPlayerState*> TopScoringPlayers;

private:
	/**
	 * 当前最高分。
	 */
	float TopScore = 0.f;
};

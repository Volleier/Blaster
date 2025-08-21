// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterGameState.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"

/**
 * 设置需要进行网络同步的属性
 * @param OutLifetimeProps 用于存储需要同步的属性列表
 */
void ABlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// 调用父类方法，确保父类属性也被同步
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 注册 TopScoringPlayers 属性进行网络同步
	DOREPLIFETIME(ABlasterGameState, TopScoringPlayers);
}

/**
 * 更新最高分玩家列表
 * @param ScoringPlayer 当前得分的玩家
 */
void ABlasterGameState::UpdateTopScore(class ABlasterPlayerState* ScoringPlayer)
{
	// 如果当前没有最高分玩家，则添加当前玩家并设置最高分
	if (TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
	// 如果当前玩家得分等于最高分，则将其加入最高分玩家列表（避免重复）
	else if (ScoringPlayer->GetScore() == TopScore)
	{
		TopScoringPlayers.AddUnique(ScoringPlayer);
	}
	// 如果当前玩家得分高于最高分，则清空列表并添加当前玩家，同时更新最高分
	else if (ScoringPlayer->GetScore() > TopScore)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.AddUnique(ScoringPlayer);
		TopScore = ScoringPlayer->GetScore();
	}
}
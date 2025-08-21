// Fill out your copyright notice in the Description page of Project Settings.

#include "BlasterGameMode.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/GameState/BlasterGameState.h"

// 定义比赛状态命名空间
namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

// 构造函数，设置延迟开始
ABlasterGameMode::ABlasterGameMode()
{
	bDelayedStart = true;
}

// 游戏开始时调用，记录关卡开始时间
void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

// 比赛状态设置时调用，通知所有玩家控制器
void ABlasterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayer)
		{
			BlasterPlayer->OnMatchStateSet(MatchState);
		}
	}
}

// 每帧调用，处理比赛倒计时和状态切换
void ABlasterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 等待开始阶段倒计时
	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
	// 比赛进行阶段倒计时
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	// 冷却阶段倒计时
	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			RestartGame();
		}
	}
}

// 玩家被淘汰时调用，处理分数和淘汰数
void ABlasterGameMode::PlayerEliminated(
	class ABlasterCharacter* ElimmedCharacter,
	class ABlasterPlayerController* VictimController,
	ABlasterPlayerController* AttackerController)
{
	// 检查控制器和玩家状态有效性
	if (AttackerController == nullptr || AttackerController->PlayerState == nullptr)
		return;
	if (VictimController == nullptr || VictimController->PlayerState == nullptr)
		return;

	ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	ABlasterPlayerState* VictimPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;

	ABlasterGameState* BlasterGameState = GetGameState<ABlasterGameState>();
	// 攻击者加分并更新最高分
	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState && BlasterGameState)
	{
		AttackerPlayerState->AddToScore(1.f);
		BlasterGameState->UpdateTopScore(AttackerPlayerState);
	}
	// 被淘汰者增加淘汰数
	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}
	// 攻击者加分（重复逻辑，可能用于特殊处理）
	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState)
	{
		AttackerPlayerState->AddToScore(1.f);
	}

	// 调用角色淘汰处理
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

// 请求重生，被淘汰角色和控制器处理
void ABlasterGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	// 重置并销毁被淘汰角色
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	// 随机选择出生点并重启玩家
	if (ElimmedController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ElimmedController valid"))
			TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}
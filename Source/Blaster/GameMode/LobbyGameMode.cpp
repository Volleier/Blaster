// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController *NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 获取当前玩家数量
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		UWorld *World = GetWorld();
		if (World)
		{
			// 使用无缝连接
			bUseSeamlessTravel = true;
			// 服务器跳转到游戏地图
			World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
		}
	}
}
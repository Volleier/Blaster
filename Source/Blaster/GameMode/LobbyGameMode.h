// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 *
 */
UCLASS()
/**
 * @class ALobbyGameMode
 * @brief 继承自AGameMode的游戏模式类，用于处理玩家登录大厅时的逻辑。
 *
 * 这个类主要用于在玩家登录到游戏大厅时执行特定的逻辑。它重写了AGameMode的PostLogin函数。
 */

/**
 * @brief 当新玩家登录时调用。
 *
 * @param NewPlayer 指向新登录玩家的APlayerController指针。
 */
class BLASTER_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void PostLogin(APlayerController *NewPlayer) override;
};
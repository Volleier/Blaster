#include "BlasterPlayerState.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Net/UnrealNetwork.h"

// 设置需要进行网络同步的属性
void ABlasterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Defeats属性需要进行网络同步
	DOREPLIFETIME(ABlasterPlayerState, Defeats);
}

// 增加分数，并更新HUD显示
void ABlasterPlayerState::AddToScore(float ScoreAmount)
{
	// 设置新的分数
	SetScore(GetScore() + ScoreAmount);

	// 获取角色指针，如果为空则尝试获取
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character)
	{
		// 获取控制器指针，如果为空则尝试获取
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			// 更新HUD上的分数显示
			Controller->SetHUDScore(GetScore());
		}
	}
}

// 增加击败数，并更新HUD显示
void ABlasterPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	// 增加击败数
	Defeats += DefeatsAmount;

	// 获取角色指针，如果为空则尝试获取
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character)
	{
		// 获取控制器指针，如果为空则尝试获取
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			// 更新HUD上的击败数显示
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

// 分数同步时的回调，更新HUD显示
void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	// 获取角色指针，如果为空则尝试获取
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character)
	{
		// 获取控制器指针，如果为空则尝试获取
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			// 更新HUD上的分数显示
			Controller->SetHUDScore(GetScore());
		}
	}
}

// 击败数同步时的回调，更新HUD显示
void ABlasterPlayerState::OnRep_Defeats()
{
	// 获取角色指针，如果为空则尝试获取
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character)
	{
		// 获取控制器指针，如果为空则尝试获取
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			// 更新HUD上的击败数显示
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

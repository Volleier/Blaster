// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

// �����ռ����ڶ������״̬��صĳ���
namespace MatchState
{
	// �����Ѵﵽ����ʱ�䣬������ȴ�׶Ρ���ʾ��ʤ�߲���ʼ��ȴ��ʱ����
	extern BLASTER_API const FName Cooldown;
}

/**
 * ABlasterGameMode
 * ��Ϸģʽ�࣬�������������̡������̭����������ʱ�ȹ��ܡ�
 */
UCLASS()
class BLASTER_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// ���캯��
	ABlasterGameMode();

	// ÿ֡���õ�Tick����
	virtual void Tick(float DeltaTime) override;

	/**
	 * ��ұ���̭ʱ����
	 * @param ElimmedCharacter ����̭�Ľ�ɫ
	 * @param VictimController ����̭��ҵĿ�����
	 * @param AttackerController �����ߵĿ�����
	 */
	virtual void PlayerEliminated(
		class ABlasterCharacter* ElimmedCharacter,
		class ABlasterPlayerController* VictimController,
		ABlasterPlayerController* AttackerController);

	/**
	 * �����������
	 * @param ElimmedCharacter ����̭�Ľ�ɫ
	 * @param ElimmedController ����̭��ɫ�Ŀ�����
	 */
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);

	// ����׼���׶�ʱ�䣨�룩
	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.f;

	// ������ʽ�׶�ʱ�䣨�룩
	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;

	// ������ȴ�׶�ʱ�䣨�룩
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.f;

	// �ؿ���ʼʱ��
	float LevelStartingTime = 0.f;

protected:
	// ��Ϸ��ʼʱ����
	virtual void BeginPlay() override;

	// ����״̬����ʱ����
	virtual void OnMatchStateSet() override;

private:
	// ��ǰ����ʱʱ�䣨�룩
	float CountdownTime = 0.f;

public:
	// ��ȡ��ǰ����ʱʱ��
	FORCEINLINE float GetCountdownTime() const { return CountdownTime; }
};
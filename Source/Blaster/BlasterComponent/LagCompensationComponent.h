#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"


// FBoxInformation 结构体用于存储命中盒的位置、旋转和范围信息
USTRUCT(BlueprintType)
struct FBoxInformation
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	FVector BoxExtent;
};

// FFramePackage 结构体用于存储某一时刻所有命中盒的信息，用于延迟补偿
USTRUCT(BlueprintType)
struct FFramePackage
{
	GENERATED_BODY()

	UPROPERTY()
	float Time;

	UPROPERTY()
	TMap<FName, FBoxInformation> HitBoxInfo;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API ULagCompensationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULagCompensationComponent();

	// 允许BlasterCharacter类访问LagCompensationComponent的私有成员
	friend class ABlasterCharacter;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 保存当前帧的命中盒信息
	void ShowFramePackage(const FFramePackage& Package, const FColor& Color);

	// 服务器端回放功能，处理命中检测和伤害应用
	void ServerSideRewind(
		class ABlasterCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize& HitLocation,
		float HitTime);

protected:
	virtual void BeginPlay() override;

	// 保存历史帧数据的数组
	void SaveFramePackage(FFramePackage& Package);

	// 帧之间插入
	FFramePackage InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime);

private:
	// Blaster角色
	UPROPERTY()
	ABlasterCharacter* Character;

	// Blaster角色的控制器
	UPROPERTY()
	class ABlasterPlayerController* Controller;

	// 历史帧数据的双向链表
	TDoubleLinkedList<FFramePackage> FrameHistory;

	// 最大记录时间（秒）
	UPROPERTY(EditAnywhere)
	float MaxRecordTime = 4.f;
};

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"


// FBoxInformation �ṹ�����ڴ洢���ке�λ�á���ת�ͷ�Χ��Ϣ
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

// FFramePackage �ṹ�����ڴ洢ĳһʱ���������ке���Ϣ�������ӳٲ���
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

	// ����BlasterCharacter�����LagCompensationComponent��˽�г�Ա
	friend class ABlasterCharacter;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ���浱ǰ֡�����к���Ϣ
	void ShowFramePackage(const FFramePackage& Package, const FColor& Color);

	// �������˻طŹ��ܣ��������м����˺�Ӧ��
	void ServerSideRewind(
		class ABlasterCharacter* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize& HitLocation,
		float HitTime);

protected:
	virtual void BeginPlay() override;

	// ������ʷ֡���ݵ�����
	void SaveFramePackage(FFramePackage& Package);

	// ֮֡�����
	FFramePackage InterpBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime);

private:
	// Blaster��ɫ
	UPROPERTY()
	ABlasterCharacter* Character;

	// Blaster��ɫ�Ŀ�����
	UPROPERTY()
	class ABlasterPlayerController* Controller;

	// ��ʷ֡���ݵ�˫������
	TDoubleLinkedList<FFramePackage> FrameHistory;

	// ����¼ʱ�䣨�룩
	UPROPERTY(EditAnywhere)
	float MaxRecordTime = 4.f;
};

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

	friend class ABlasterCharacter;

protected:
	virtual void BeginPlay() override;

	// Blaster��ɫ
	UPROPERTY()
	ABlasterCharacter* Character;

	// Blaster��ɫ�Ŀ�����
	UPROPERTY()
	class ABlasterPlayerController* Controller;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileRocket.h"
#include "Kismet/GameplayStatics.h"

AProjectileRocket::AProjectileRocket()
{
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket Mesh"));
	RocketMesh->SetupAttachment(RootComponent);
	RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	APawn* FiringPawn = GetInstigator();
	if (FiringPawn)
	{
		AController* FiringController = FiringPawn->GetController();
		if (FiringController)
		{
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				this, // ���������Ķ���
				Damage, // �����˺�
				10.f, // ��С�˺�
				GetActorLocation(), // ԭ����
				200.f, // �˺��뾶
				500.f, // �˺��뾶������˺�
				1.f, // �˺�˥��
				UDamageType::StaticClass(), // �˺�����
				TArray<AActor*>(), // �����б�
				this, // �˺���
				FiringController // ������
			);
		}
	}
}
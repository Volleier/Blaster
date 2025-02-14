// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Blaster.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true; // 设置该Actor是否每帧调用Tick函数
	bReplicates = true;					  // 设置该Actor是否进行网络同步

	// 创建碰撞盒组件并设置为根组件
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	// 设置碰撞盒的碰撞属性
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);

	// 创建弹道运动组件并设置其属性
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

	// 如果拥有服务器权限，绑定碰撞事件
	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// 如果Tracer有效，生成并附加Tracer粒子效果
	if (Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition);
	}
}

// 子弹碰撞后销毁
void AProjectile::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit)
{
	// 如果碰撞到BlasterCharacter，调用其MulticastHit函数
	ABlasterCharacter *BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (BlasterCharacter)
	{
		BlasterCharacter->MulticastHit();
	}

	// 销毁子弹
	Destroy();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 重写销毁函数
void AProjectile::Destroyed()
{
	Super::Destroyed();

	// 如果ImpactParticles有效，生成碰撞粒子效果
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	// 如果ImpactSound有效，播放碰撞音效
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

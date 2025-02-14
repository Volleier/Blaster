// Fill out your copyright notice in the Description page of Project Settings.

#include "Casing.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ACasing::ACasing()
{
	// 设置此Actor每帧调用Tick()。如果不需要，可以将其设置为false以提高性能。
	PrimaryActorTick.bCanEverTick = true;

	// 创建并初始化CasingMesh组件
	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	// 将CasingMesh设置为根组件
	SetRootComponent(CasingMesh);
	// 设置CasingMesh对摄像机通道的碰撞响应为忽略
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	// 启用物理模拟
	CasingMesh->SetSimulatePhysics(true);
	// 启用重力
	CasingMesh->SetEnableGravity(true);
	// 启用刚体碰撞通知
	CasingMesh->SetNotifyRigidBodyCollision(true);
	// 设置弹壳抛出冲量
	ShellEjectionImpulse = 10.f;
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();

	// 绑定碰撞事件
	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);
	// 给弹壳添加一个向前的冲量
	CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);
}

void ACasing::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit)
{
	// 如果ShellSound不为空，则在弹壳位置播放声音
	if (ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
	}
	// 销毁弹壳
	Destroy();
}
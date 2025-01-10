// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "Casing.h"
#include "Engine/SkeletalMeshSocket.h"

// 设置默认值
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false; // 禁用每帧调用Tick函数
	bReplicates = true;					   // 启用网络复制

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh")); // 创建武器网格组件
	SetRootComponent(WeaponMesh);													 // 设置根组件为武器网格

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);							// 设置武器网格对所有通道的碰撞响应为阻挡
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore); // 忽略与Pawn通道的碰撞
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);										// 禁用碰撞

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));	   // 创建球体组件
	AreaSphere->SetupAttachment(RootComponent);									   // 将球体组件附加到根组件
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // 忽略所有通道的碰撞响应
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);			   // 禁用碰撞

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget")); // 创建拾取小部件组件
	PickupWidget->SetupAttachment(RootComponent);								   // 将拾取小部件附加到根组件
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) // 如果拥有控制权
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);									 // 启用查询和物理碰撞
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); // 设置与Pawn通道的碰撞响应为重叠
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);						 // 添加重叠开始事件处理函数
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);						 // 添加重叠结束事件处理函数
	}
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false); // 隐藏拾取小部件
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 注册需要复制的属性
void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
}

// 设置武器状态
void AWeapon::OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	ABlasterCharacter *BlasterCharacter = Cast<ABlasterCharacter>(OtherActor); // 将重叠的Actor转换为BlasterCharacter
	if (BlasterCharacter)
	{
		BlasterCharacter->SetOverlappingWeapon(this); // 设置重叠的武器
	}
}

// 清除重叠的武器
void AWeapon::OnSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	ABlasterCharacter *BlasterCharacter = Cast<ABlasterCharacter>(OtherActor); // 将重叠的Actor转换为BlasterCharacter
	if (BlasterCharacter)
	{
		BlasterCharacter->SetOverlappingWeapon(nullptr); // 清除重叠的武器
	}
}

// 设置武器状态
void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State; // 设置武器状态
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);										 // 隐藏拾取小部件
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 禁用碰撞
		break;
	}
}

// 复制武器状态
void AWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false); // 隐藏拾取小部件
		break;
	}
}

// 显示或隐藏拾取小部件
void AWeapon::ShowPickupWidget(bool bShowWidget)
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget); // 设置拾取小部件的可见性
	}
}

// 开火
void AWeapon::Fire(const FVector &HitTargets)
{
	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false); // 播放开火动画
	}
	if (CasingClass)
	{
		const USkeletalMeshSocket* AmmoEjectSocket = WeaponMesh->GetSocketByName(FName("AmmoEject"));
		if (AmmoEjectSocket)
		{
			FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(WeaponMesh);

			FActorSpawnParameters SpawnParams;
			UWorld* World = GetWorld();
			if (World)
			{
				World->SpawnActor<ACasing>(
					CasingClass,
					SocketTransform.GetLocation(),
					SocketTransform.GetRotation().Rotator()
				);
			}
		}

	}
}

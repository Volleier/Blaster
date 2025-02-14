// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"

#include "Engine/Engine.h"

// todo : FIX ME
void AProjectileWeapon::Fire(const FVector &HitTarget)
{
	Super::Fire(HitTarget);

	if (!HasAuthority())
		return;
	//(X=72.565056,Y=0.000000,Z=11.347687)
	APawn *InstigatorPawn = Cast<APawn>(GetOwner());

	FTransform MuzzleBoneTransform = GetWeaponMesh()->GetBoneTransform(GetWeaponMesh()->GetBoneIndex(FName("Grip_Bone")));
	UE_LOG(LogTemp, Warning, TEXT("Grip_Bone Location: %s"), *MuzzleBoneTransform.GetLocation().ToString());
	FVector FireLocation = MuzzleBoneTransform.GetLocation() + FVector(72.565056f, 0.0f, 11.347687f);
	UE_LOG(LogTemp, Warning, TEXT("FireLocation: %s"), *FireLocation.ToString());

	FVector ToTarget = HitTarget - FireLocation;
	FRotator TargetRotation = ToTarget.Rotation();
	if (ProjectileClass && InstigatorPawn)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = InstigatorPawn;
		UWorld *World = GetWorld();
		if (World)
		{
			// 生成子弹
			World->SpawnActor<AProjectile>(
				ProjectileClass,
				FireLocation,
				TargetRotation,
				SpawnParams);
		}
	}

	// 添加调试可视化
	// 从枪口位置到屏幕中心位置连线
	DrawDebugLine(
		GetWorld(),
		FireLocation,
		HitTarget,
		FColor::Red,
		false,
		2.0f);
	// 在屏幕中心位置画一个红色球
	DrawDebugSphere(
		GetWorld(),
		HitTarget,
		10.0f,
		12,
		FColor::Red,
		false,
		2.0f);
	// 绘制球体标记位置
	DrawDebugSphere(
		GetWorld(),
		FireLocation,
		10.f,
		8,
		FColor::Red,
		false,
		2.f,
		0,
		2.f);
}

#pragma region Waste Code (Socket)
/*
APawn *InstigatorPawn = Cast<APawn>(GetOwner());
const USkeletalMeshSocket *MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
if (MuzzleFlashSocket)
{
	FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	FVector SocketLocation = SocketTransform.GetLocation();
	// 从枪口位置到屏幕中心位置连线
	// HitTarget 来自准星射线检测
	FVector ToTarget = HitTarget - SocketTransform.GetLocation();
	FRotator TargetRotation = ToTarget.Rotation();
	if (ProjectileClass && InstigatorPawn)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = InstigatorPawn;
		UWorld *World = GetWorld();
		if (World)
		{
			// 生成子弹
			World->SpawnActor<AProjectile>(
				ProjectileClass,
				SocketTransform.GetLocation(),
				TargetRotation,
				SpawnParams);
		}
	}

	// 添加调试可视化
	// 从枪口位置到屏幕中心位置连线
	DrawDebugLine(
		GetWorld(),
		SocketTransform.GetLocation(),
		HitTarget,
		FColor::Red,
		false,
		2.0f);
	// 在屏幕中心位置画一个红色球
	DrawDebugSphere(
		GetWorld(),
		HitTarget,
		10.0f,
		12,
		FColor::Red,
		false,
		2.0f);
	// 绘制球体标记位置
	DrawDebugSphere(
		GetWorld(),
		SocketTransform.GetLocation(),
		10.f,
		8,
		FColor::Red,
		false,
		2.f,
		0,
		2.f);
}
*/
#pragma endregion

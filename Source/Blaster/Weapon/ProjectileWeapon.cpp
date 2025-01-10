// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"

void AProjectileWeapon::Fire(const FVector &HitTarget)
{
	Super::Fire(HitTarget);

	if (!HasAuthority())
		return;

	APawn *InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket *MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector SocketLocation = SocketTransform.GetLocation();

		// to-do fix the problem of the socket location

		
		UE_LOG(LogTemp, Warning, TEXT("Weapon Socket Location: X=%f, Y=%f, Z=%f"),
			   SocketLocation.X, SocketLocation.Y, SocketLocation.Z);

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
}

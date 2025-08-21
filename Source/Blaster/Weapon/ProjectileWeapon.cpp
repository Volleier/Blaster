#include "ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

// todo : FIX ME
// 发射子弹的函数
void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	// 调用父类的 Fire 方法
	Super::Fire(HitTarget);

	// 仅在服务器端执行发射逻辑
	if (!HasAuthority())
		return;

	// 获取武器的拥有者（角色 Pawn）
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());

	// 获取武器网格上名为 "Grip_Bone" 的骨骼的变换信息
	FTransform MuzzleBoneTransform = GetWeaponMesh()->GetBoneTransform(GetWeaponMesh()->GetBoneIndex(FName("Grip_Bone")));
	UE_LOG(LogTemp, Warning, TEXT("Grip_Bone Location: %s"), *MuzzleBoneTransform.GetLocation().ToString());

	// 计算实际的发射位置（在骨骼位置基础上偏移一定距离）
	FVector FireLocation = MuzzleBoneTransform.GetLocation() + FVector(72.565056f, 0.0f, 11.347687f);
	UE_LOG(LogTemp, Warning, TEXT("FireLocation: %s"), *FireLocation.ToString());

	// 计算从发射点到目标点的方向
	FVector ToTarget = HitTarget - FireLocation;
	FRotator TargetRotation = ToTarget.Rotation();

	// 如果子弹类和拥有者有效，则生成子弹
	if (ProjectileClass && InstigatorPawn)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = InstigatorPawn;
		UWorld* World = GetWorld();
		if (World)
		{
			// 生成子弹实例
			World->SpawnActor<AProjectile>(
				ProjectileClass,
				FireLocation,
				TargetRotation,
				SpawnParams);
		}
	}

	// 添加调试可视化
	// 绘制从枪口位置到目标点的红色线段
	DrawDebugLine(
		GetWorld(),
		FireLocation,
		HitTarget,
		FColor::Red,
		false,
		2.0f);

	// 在目标点绘制一个红色球体
	DrawDebugSphere(
		GetWorld(),
		HitTarget,
		10.0f,
		12,
		FColor::Red,
		false,
		2.0f);

	// 在发射点绘制一个红色球体
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

#pragma region 废弃代码（使用 Socket 方式）
// 以下为废弃的代码片段，使用枪口 Socket 获取发射位置
/*
APawn *InstigatorPawn = Cast<APawn>(GetOwner());
const USkeletalMeshSocket *MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
if (MuzzleFlashSocket)
{
	FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	FVector SocketLocation = SocketTransform.GetLocation();
	// 计算从枪口位置到目标点的方向
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
			// 生成子弹实例
			World->SpawnActor<AProjectile>(
				ProjectileClass,
				SocketTransform.GetLocation(),
				TargetRotation,
				SpawnParams);
		}
	}

	// 绘制从枪口位置到目标点的红色线段
	DrawDebugLine(
		GetWorld(),
		SocketTransform.GetLocation(),
		HitTarget,
		FColor::Red,
		false,
		2.0f);

	// 在目标点绘制一个红色球体
	DrawDebugSphere(
		GetWorld(),
		HitTarget,
		10.0f,
		12,
		FColor::Red,
		false,
		2.0f);

	// 在枪口位置绘制一个红色球体
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

// 填写您的版权声明在项目设置的描述页面

#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Blaster.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// 构造函数
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true; // 设置该Actor每帧调用Tick函数
	bReplicates = true;					  // 设置该Actor进行网络同步

	// 创建碰撞盒组件并设置为根组件
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);

	// 设置碰撞盒的碰撞属性
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);										// 设置对象类型为动态
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);											// 启用查询和物理碰撞
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);								// 忽略所有通道
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);	// 可见性通道阻挡
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block); // 静态世界通道阻挡
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);					// 骨骼网格通道阻挡

	// 如果拥有服务器权限，绑定碰撞事件
	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
}

// 游戏开始时调用
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// 如果Tracer有效，生成并附加Tracer粒子效果
	if (Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,							   // 粒子系统
			CollisionBox,					   // 附加到碰撞盒
			FName(),						   // 附加点名称
			GetActorLocation(),				   // 位置
			GetActorRotation(),				   // 旋转
			EAttachLocation::KeepWorldPosition // 保持世界位置
		);
	}

	// 服务器权限下再次绑定碰撞事件（冗余，确保绑定）
	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
}

// 生成拖尾特效
void AProjectile::SpawnTrailSystem()
{
	if (TrailSystem)
	{
		TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			TrailSystem,						// 拖尾特效系统
			GetRootComponent(),					// 附加到根组件
			FName(),							// 附加点名称
			GetActorLocation(),					// 位置
			GetActorRotation(),					// 旋转
			EAttachLocation::KeepWorldPosition, // 保持世界位置
			false								// 不自动销毁
		);
	}
}

// 爆炸伤害处理
void AProjectile::ExplodeDamage()
{
	APawn* FiringPawn = GetInstigator(); // 获取发射者
	if (FiringPawn && HasAuthority())
	{
		AController* FiringController = FiringPawn->GetController(); // 获取控制器
		if (FiringController)
		{
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				this,						// 世界上下文对象
				Damage,						// 基础伤害
				10.f,						// 最小伤害
				GetActorLocation(),			// 爆炸中心
				DamageInnerRadius,			// 内半径
				DamageOuterRadius,			// 外半径
				1.f,						// 伤害衰减
				UDamageType::StaticClass(), // 伤害类型
				TArray<AActor*>(),			// 忽略的Actor列表
				this,						// 伤害制造者
				FiringController			// 发射者控制器
			);
		}
	}
}

// 启动销毁计时器
void AProjectile::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer(
		DestroyTimer,						// 计时器句柄
		this,								// 目标对象
		&AProjectile::DestroyTimerFinished, // 回调函数
		DestroyTime							// 延迟时间
	);
}

// 销毁计时器回调
void AProjectile::DestroyTimerFinished()
{
	Destroy(); // 销毁自身
}

// 子弹碰撞后销毁
void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy(); // 碰撞后销毁自身
}

// 每帧调用
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

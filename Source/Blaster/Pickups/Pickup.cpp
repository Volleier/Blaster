#include "Pickup.h"
#include "Kismet/GameplayStatics.h"		// 用于游戏相关的静态方法（如播放音效）
#include "Sound/SoundCue.h"				// 声音提示相关
#include "Components/SphereComponent.h" // 球体碰撞组件
#include "Blaster/Weapon/WeaponTypes.h" // 武器类型定义
#include "NiagaraComponent.h"			// Niagara特效组件
#include "NiagaraFunctionLibrary.h"		// Niagara特效库函数

// 构造函数
APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true; // 允许Tick函数每帧调用
	bReplicates = true;					  // 支持网络同步

	// 创建根组件
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// 创建并设置球体碰撞组件
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(RootComponent);																// 附加到根组件
	OverlapSphere->SetSphereRadius(150.f);																		// 设置半径
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);											// 仅用于查询（不产生物理碰撞）
	OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);							// 忽略所有通道
	OverlapSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); // 仅对Pawn产生重叠响应
	OverlapSphere->AddLocalOffset(FVector(0.f, 0.f, 85.f));														// 设置本地偏移

	// 创建并设置拾取物体的静态网格组件
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(OverlapSphere);						 // 附加到球体碰撞组件
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 不产生碰撞
	PickupMesh->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));			 // 设置缩放
	PickupMesh->SetRenderCustomDepth(true);							 // 启用自定义深度渲染
	PickupMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_PURPLE);	 // 设置自定义深度模板值

	// 创建并设置Niagara特效组件
	PickupEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PickupEffectComponent"));
	PickupEffectComponent->SetupAttachment(RootComponent); // 附加到根组件
}

// 游戏开始时调用
void APickup::BeginPlay()
{
	Super::BeginPlay();

	// 仅在服务器端绑定重叠事件
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			BindOverlapTimer,
			this,
			&APickup::BindOverlapTimerFinished,
			BindOverlapTime
		);
	}
}

// 球体重叠事件回调（留空，待实现）
void APickup::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
}

// 每帧调用
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PickupMesh)
	{
		// 让拾取物体旋转
		PickupMesh->AddWorldRotation(
			FRotator(0.f, 
				BaseTurnRate * 
				DeltaTime, 0.f
			)
		);
	}
}

// 拾取物体销毁时调用
void APickup::Destroyed()
{
	Super::Destroyed();

	// 播放拾取音效
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			PickupSound,
			GetActorLocation());
	}
	// 生成拾取特效
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation(),
			GetActorRotation());
	}
}

// 定时器结束后绑定重叠事件
void APickup::BindOverlapTimerFinished()
{
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(
		this,
		&APickup::OnSphereOverlap
	);
}
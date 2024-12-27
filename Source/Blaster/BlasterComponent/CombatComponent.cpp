// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	BaseWalkSpeed = 600.f; // 基础行走速度
	AimWalkSpeed = 450.f;  // 瞄准行走速度
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed; // 设置角色的最大行走速度为基础行走速度
	}
}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed; // 根据是否瞄准设置角色的最大行走速度
	}
}

void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed; // 根据是否瞄准设置角色的最大行走速度
	}
}

void UCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character)
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement = false; // 禁用角色移动时的旋转
		Character->bUseControllerRotationYaw = true;						  // 启用控制器旋转
	}
}

void UCombatComponent::FireButtonPressed(bool bIsPressed)
{
	bFireButtonPressed = bIsPressed;
	if (EquippedWeapon == nullptr)return;
	if (Character && bFireButtonPressed)
	{
		Character->PlayFireMonatge(bAiming);
		EquippedWeapon->Fire();
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedWeapon); // 复制装备的武器
	DOREPLIFETIME(UCombatComponent, bAiming);		 // 复制瞄准状态
}

void UCombatComponent::EquipWeapon(AWeapon *WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr)
		return;

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped); // 设置武器状态为已装备
	const USkeletalMeshSocket *HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh()); // 将武器附加到角色的右手插槽
	}
	EquippedWeapon->SetOwner(Character);								  // 设置武器的拥有者为角色
	Character->GetCharacterMovement()->bOrientRotationToMovement = false; // 禁用角色移动时的旋转
	Character->bUseControllerRotationYaw = true;						  // 启用控制器旋转
}
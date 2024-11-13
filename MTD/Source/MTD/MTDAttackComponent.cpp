// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "MTDAttackComponent.h"
#include "Net/UnrealNetwork.h"
#include "MTD.h"

UMTDAttackComponent::UMTDAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	AttackDamage = -1.0f;
	AttackRange = -1.0f;
	AttackSpeed = -1.0f;
	OwnerActor = nullptr;
}

void UMTDAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMTDAttackComponent, AttackDamage);
	DOREPLIFETIME(UMTDAttackComponent, AttackRange);
	DOREPLIFETIME(UMTDAttackComponent, AttackSpeed);
}

void UMTDAttackComponent::Attack_Server_Reliable_Implementation()
{
	// Default implementation for the base attack server logic (can be empty if overridden)
}

bool UMTDAttackComponent::Attack_Server_Reliable_Validate()
{
	// Default validation (returns true if no specific validation is required)
	return true;
}

void UMTDAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMTDAttackComponent::OnRep_AttackDamage()
{
	// Optional: Add code to respond to changes in AttackDamage
}

void UMTDAttackComponent::OnRep_AttackRange()
{
	// Optional: Add code to respond to changes in AttackRange
}

void UMTDAttackComponent::OnRep_AttackSpeed()
{
	// Optional: Add code to respond to changes in AttackSpeed
}
// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#include "CombatEntity.h"
#include "CombatEntityManager.h"
#include <Kismet/GameplayStatics.h>
#include "CustomGameMode.h"
#include "MTD.h"


ACombatEntity::ACombatEntity()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	CurrentHealth = 100.0f;
	MaxHealth = 100.0f;
	AttackDamage = 10.0f;
	AttackRange = 500.0f;
	AttackSpeed = 1.0f;
	bIsDead = false;
}

void ACombatEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatEntity::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		ACustomGameMode* GameMode = Cast<ACustomGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			CombatEntityManager = GameMode->GetCombatEntityManager();
			if (CombatEntityManager)
			{
				CombatEntityManager->RegisterEntity(this);
			}
			else
			{
				UE_LOG(MTDLog, Warning, TEXT("No CombatEntityManager found in the level for entity registration."));
			}
		}
		else
		{
			UE_LOG(MTDLog, Error, TEXT("No GameMode found in the level!"))
		}
	}
}

void ACombatEntity::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HasAuthority() && CombatEntityManager)
	{
		CombatEntityManager->UnRegisterEntity(this);
	}

	Super::EndPlay(EndPlayReason);

}

void ACombatEntity::RegisterManager(ACombatEntityManager* CombatEntityManagerRef)
{
	if (CombatEntityManagerRef == nullptr)
	{
		UE_LOG(MTDLog, Warning, TEXT("The CombatManager register failed to to invalid CombatEntityManager pointer!"))
	}
	CombatEntityManager = CombatEntityManagerRef;
}

void ACombatEntity::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Add each property to be replicated
	DOREPLIFETIME(ACombatEntity, CurrentHealth);
	DOREPLIFETIME(ACombatEntity, MaxHealth);
	DOREPLIFETIME(ACombatEntity, AttackDamage);
	DOREPLIFETIME(ACombatEntity, AttackRange);
	DOREPLIFETIME(ACombatEntity, AttackSpeed);
	DOREPLIFETIME(ACombatEntity, bIsDead);
	DOREPLIFETIME(ACombatEntity, CombatEntityManager);
}

void ACombatEntity::ServerAttack_Implementation()
{
	UE_LOG(MTDLog, Warning, TEXT("ServerAttack called on ACombatEntity, but this function should be overridden in a derived class."));
}

bool ACombatEntity::ServerAttack_Validate()
{
	return true;
}


void ACombatEntity::ServerTakeDamage_Implementation(float DamageAmount)
{
	if (HasAuthority())
	{
		CurrentHealth -= DamageAmount;
		if (CurrentHealth <= 0.0f)
		{
			CurrentHealth = 0.0f;
			MulticastOnDeath();
		}
	}
}

bool ACombatEntity::ServerTakeDamage_Validate(float DamageAmount)
{
	return DamageAmount > 0.0f;
}


void ACombatEntity::ServerHeal_Implementation(float HealAmount)
{
	if (HasAuthority())
	{
		CurrentHealth += HealAmount;
		if (CurrentHealth > MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}
	}
}

bool ACombatEntity::ServerHeal_Validate(float HealAmount)
{
	return HealAmount > 0.0f;
}


void ACombatEntity::MulticastOnDeath_Implementation()
{
	bIsDead = true;
	UE_LOG(MTDLog, Warning, TEXT("CombatEntity has died!"));
}


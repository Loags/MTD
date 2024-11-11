// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "BaseEnemy.h"
#include <Net/UnrealNetwork.h>
#include "CustomGameState.h"
#include "MTD.h"

ABaseEnemy::ABaseEnemy()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	CurrentHealth = 100.0f;
	MaxHealth = 100.0f;
	AttackDamage = 10.0f;
	AttackRange = 500.0f;
	AttackSpeed = 1.0f;
	bIsDead = false;
	EntityManager = nullptr;
}

void ABaseEnemy::BeginPlay()
{
	if (HasAuthority())
	{
		ACustomGameState* GameState = GetWorld()->GetGameState<ACustomGameState>();
		if (GameState)
		{
			UE_LOG(MTDLog, Warning, TEXT("GameState cast successful!"));

			EntityManager = GameState->GetEntityManager();
			if (EntityManager)
			{
				EntityManager->RegisterEnemy(this);
			}
			else
			{
				UE_LOG(MTDLog, Warning, TEXT("No CombatEntityManager found in the level for entity registration."));
			}
		}
		else
		{
			UE_LOG(MTDLog, Error, TEXT("GameState cast failed!"));
			return;
		}
	}
}


void ABaseEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseEnemy, CurrentHealth);
	DOREPLIFETIME(ABaseEnemy, MaxHealth);
	DOREPLIFETIME(ABaseEnemy, AttackDamage);
	DOREPLIFETIME(ABaseEnemy, AttackRange);
	DOREPLIFETIME(ABaseEnemy, AttackSpeed);
	DOREPLIFETIME(ABaseEnemy, bIsDead);
	DOREPLIFETIME(ABaseEnemy, EntityManager);
}

void ABaseEnemy::Attack_Server_Reliable_Implementation(ABaseTower* Tower)
{
	if (HasAuthority())
	{
		UE_LOG(MTDLog, Warning, TEXT("Tower is attacking!"));
	}
}

void ABaseEnemy::TakeDamage_Server_Reliable_Implementation(float DamageAmount)
{
	if (HasAuthority())
	{
		CurrentHealth -= DamageAmount;
		if (CurrentHealth <= 0.0f)
		{
			CurrentHealth = 0.0f;
			OnDeath_Multicast_Reliable_Implementation();
		}
	}
}

void ABaseEnemy::Heal_Server_Reliable_Implementation(float HealAmount)
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

void ABaseEnemy::OnDeath_Multicast_Reliable_Implementation()
{
	bIsDead = true;
	UE_LOG(MTDLog, Warning, TEXT("Enemy has died!"));
}

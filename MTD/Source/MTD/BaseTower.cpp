// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#include "BaseTower.h"
#include "MTD.h"
#include "TimerManager.h" 
#include "DrawDebugHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "CustomGameState.h"
#include "BaseEnemy.h"


ABaseTower::ABaseTower()
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
	CurrentTargetEnemy = nullptr;
	EnemySearchInterval = 0.2f;
}

void ABaseTower::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseTower, CurrentHealth);
	DOREPLIFETIME(ABaseTower, MaxHealth);
	DOREPLIFETIME(ABaseTower, AttackDamage);
	DOREPLIFETIME(ABaseTower, AttackRange);
	DOREPLIFETIME(ABaseTower, AttackSpeed);
	DOREPLIFETIME(ABaseTower, bIsDead);
	DOREPLIFETIME(ABaseTower, EntityManager);
}


void ABaseTower::BeginPlay()
{
	if (HasAuthority())
	{
		ACustomGameState* GameState = GetWorld()->GetGameState<ACustomGameState>();
		if (GameState)
		{
			EntityManager = GameState->GetEntityManager();
			if (EntityManager)
			{
				EntityManager->RegisterTower(this);
			}
			else
			{
				UE_LOG(MTDLog, Warning, TEXT("No EntityManager found in the level for entity registration."));
			}
		}
		else
		{
			UE_LOG(MTDLog, Error, TEXT("No GameState found in the level!"));
			return;
		}

		GetWorld()->GetTimerManager().SetTimer(EnemySearchTimerHandle, this, &ABaseTower::FindClosestEnemy_Server_Reliable, EnemySearchInterval, true);
		GetWorld()->GetTimerManager().SetTimer(TowerAttackTimerHandle, this, &ABaseTower::AttackIntervalCheck, AttackSpeed, true);
	}
}

void ABaseTower::Attack_Server_Reliable_Implementation()
{
	if (HasAuthority() && CurrentTargetEnemy)
	{
		CurrentTargetEnemy->TakeDamage_Server_Reliable(AttackDamage);
		UE_LOG(MTDLog, Warning, TEXT("Tower is attacking!"));
	}
}

bool ABaseTower::Attack_Server_Reliable_Validate()
{
	if (!HasAuthority()) return false;

	// Ensure the tower is not dead and has a valid target within range
	if (bIsDead || !CurrentTargetEnemy)
	{
		UE_LOG(MTDLog, Warning, TEXT("Attack_Server_Reliable validation failed: Tower is dead or has no valid target."));
		return false;
	}

	// Check that the enemy is within range
	float DistanceToTarget = FVector::Dist(GetActorLocation(), CurrentTargetEnemy->GetActorLocation());
	if (DistanceToTarget > GetAttackRange())
	{
		UE_LOG(MTDLog, Warning, TEXT("Attack_Server_Reliable validation failed: Target is out of range."));
		return false;
	}

	return true; // Validation passed
}

void ABaseTower::TakeDamage_Server_Reliable_Implementation(float DamageAmount)
{
	if (HasAuthority())
	{
		CurrentHealth -= DamageAmount;
		if (CurrentHealth <= 0.0f)
		{
			CurrentHealth = 0.0f;
			OnDeath_Multicast_Reliable();
		}
	}
}

void ABaseTower::Heal_Server_Reliable_Implementation(float HealAmount)
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

void ABaseTower::OnDeath_Multicast_Reliable_Implementation()
{
	bIsDead = true;
	UE_LOG(MTDLog, Warning, TEXT("Tower has died!"));
}


void ABaseTower::FindClosestEnemy_Server_Reliable_Implementation()
{
	float ClosestDistance = GetAttackRange() * 2;
	ABaseEnemy* ClosestEnemy = nullptr;

	if (EntityManager)
	{
		for (ABaseEnemy* Enemy : EntityManager->GetEnemies())
		{
			if (Enemy)
			{
				float Distance = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());

				if (Distance <= GetAttackRange() && Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestEnemy = Enemy;
				}
			}
			else
			{
				UE_LOG(MTDLog, Warning, TEXT("Encountered a null enemy in CombatEntityManager's list."));
			}
		}
	}
	else
	{
		UE_LOG(MTDLog, Error, TEXT("CombatEntityManager is nullptr. Cannot find enemies."));
	}

	if (ClosestEnemy)
	{
		DrawClosesEnemyLine_Multicast_Reliable(this, ClosestEnemy, GetWorld(), FColor::Red, 2.0f, EnemySearchInterval);
	}

	CurrentTargetEnemy = ClosestEnemy;
}

void ABaseTower::DrawClosesEnemyLine_Multicast_Reliable_Implementation(AActor* Actor1, AActor* Actor2, UWorld* World, FColor LineColor, float LineThickness, float Duration)
{
	DrawLineBetweenActors(Actor1, Actor2, World, LineColor, LineThickness, Duration);
}

void ABaseTower::AttackIntervalCheck()
{
	if (!CurrentTargetEnemy || !HasAuthority()) return;

	Attack_Server_Reliable();
}

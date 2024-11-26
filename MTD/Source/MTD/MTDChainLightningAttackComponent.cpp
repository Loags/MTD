// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "MTDChainLightningAttackComponent.h"
#include "MTD.h"
#include "BaseEnemy.h"
#include "BaseTower.h"
#include "MTDEnemyHealthComponent.h"
#include "EntityManager.h"
#include "TimerManager.h" 


UMTDChainLightningAttackComponent::UMTDChainLightningAttackComponent()
{
	AttackDamage = 30.0f;  // Initial attack damage
	DamageReductionPerHit = 0.2f; // Reduce damage by 20% per hit
	MaxChainHits = 4;      // Max 4 enemies in a chain
}

void UMTDChainLightningAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerActor->HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(EnemySearchTimerHandle, this, &UMTDChainLightningAttackComponent::FindEnemyToAttack_Server_Reliable, EnemySearchInterval, true);
		GetWorld()->GetTimerManager().SetTimer(TowerAttackTimerHandle, this, &UMTDChainLightningAttackComponent::AttackIntervalCheck, AttackSpeed, true);
	}
}

void UMTDChainLightningAttackComponent::Attack_Server_Reliable_Implementation()
{
	if (!CurrentTargetEnemy)
	{
		UE_LOG(MTDLog, Warning, TEXT("[%s] - UMTDChainLightningAttackComponent::Attack_Server_Reliable_Implementation - No target to attack"),
			*FString(OwnerActor->GetName()));
		return;
	}

	float CurrentDamage = AttackDamage;
	ABaseEnemy* CurrentEnemy = CurrentTargetEnemy;
	TArray<ABaseEnemy*> EnemiesHit;
	EnemiesHit.Add(CurrentEnemy);

	for (int32 HitCount = 0; HitCount < MaxChainHits; ++HitCount)
	{
		if (!CurrentEnemy) break;

		// Apply damage to the current enemy
		CurrentEnemy->GetEnemyHealthComponent()->TakeDamage_Server_Reliable(CurrentDamage);

		// Reduce damage for the next enemy
		CurrentDamage *= (1.0f - DamageReductionPerHit);

		// Find the next enemy to chain to within range, excluding already hit enemies
		ABaseEnemy* NextEnemy = nullptr;
		float ClosestDistance = AttackRange;

		if (BaseTower && BaseTower->GetEntityManager())
		{
			for (ABaseEnemy* Enemy : BaseTower->GetEntityManager()->GetEnemies())
			{
				if (Enemy && !EnemiesHit.Contains(Enemy))
				{
					float Distance = FVector::Dist(CurrentEnemy->GetActorLocation(), Enemy->GetActorLocation());
					if (Distance <= AttackRange && Distance < ClosestDistance)
					{
						ClosestDistance = Distance;
						NextEnemy = Enemy;
					}
				}
			}
		}

		if (NextEnemy)
		{
			EnemiesHit.Add(NextEnemy);
			CurrentEnemy = NextEnemy;
		}
		else
		{
			UE_LOG(MTDLog, Warning, TEXT("[%s] - UMTDChainLightningAttackComponent::ApplyChainDamage - No more enemies in range"),
				*FString(OwnerActor->GetName()));

			break; // No more enemies to chain to
		}
	}
}
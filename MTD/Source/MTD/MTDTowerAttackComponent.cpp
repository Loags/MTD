// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "MTDTowerAttackComponent.h"
#include "Net/UnrealNetwork.h"
#include "MTD.h"
#include "BaseEnemy.h"
#include "BaseTower.h"
#include "MTDEnemyHealthComponent.h"
#include "EntityManager.h"
#include "TimerManager.h" 

UMTDTowerAttackComponent::UMTDTowerAttackComponent()
{
	AttackDamage = 10.0f;
	AttackRange = 500.0f;
	AttackSpeed = 1.0f;
	OwnerActor = nullptr;
	BaseTower = nullptr;
	EnemySearchInterval = 1.0f;
	BaseTower = nullptr;
	CurrentTargetEnemy = nullptr;
	TargetingMode = ETowerTargetingMode::FirstTarget;
}

void UMTDTowerAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = GetOwner();
	GetAttackRangeDecal();
	SetAttackRangeDecalValues();

	/*if (OwnerActor->HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(EnemySearchTimerHandle, this, &UMTDTowerAttackComponent::FindEnemyToAttack_Server_Reliable, EnemySearchInterval, true);
		GetWorld()->GetTimerManager().SetTimer(TowerAttackTimerHandle, this, &UMTDTowerAttackComponent::AttackIntervalCheck, AttackSpeed, true);
	}*/
}

void UMTDTowerAttackComponent::Attack_Server_Reliable_Implementation()
{
	if (!OwnerActor->HasAuthority() || !CurrentTargetEnemy) return;

	CurrentTargetEnemy->GetEnemyHealthComponent()->TakeDamage_Server_Reliable(AttackDamage);
}

bool UMTDTowerAttackComponent::Attack_Server_Reliable_Validate()
{
	if (!OwnerActor->HasAuthority()) return false;

	// Ensure the tower is not dead and has a valid target within range
	if (BaseTower->GetHealthComponent()->GetIsDead())
	{
		UE_LOG(MTDLog, Warning, TEXT("[%s] - ABaseTower::Attack_Server_Reliable_Validate - Tower is dead: on %s"),
			*FString(OwnerActor->HasAuthority() ? TEXT("Server") : TEXT("Client")),
			*GetOwner()->GetName());

		return false;
	}

	if (!CurrentTargetEnemy)
	{
		UE_LOG(MTDLog, Warning, TEXT("[%s] - ABaseTower::Attack_Server_Reliable_Validate - Tower has no valid targets: on %s"),
			*FString(OwnerActor->HasAuthority() ? TEXT("Server") : TEXT("Client")),
			*GetOwner()->GetName());

		return false;
	}

	// Check that the enemy is within range
	float DistanceToTarget = FVector::Dist(OwnerActor->GetActorLocation(), CurrentTargetEnemy->GetActorLocation());
	if (DistanceToTarget > GetAttackRange())
	{
		UE_LOG(MTDLog, Warning, TEXT("[%s] - ABaseTower::Attack_Server_Reliable_Validate - Target is out of range: on %s"),
			*FString(OwnerActor->HasAuthority() ? TEXT("Server") : TEXT("Client")),
			*GetOwner()->GetName());

		return false;
	}

	return true;
}

void UMTDTowerAttackComponent::OnRep_AttackDamage()
{
}

void UMTDTowerAttackComponent::OnRep_AttackRange()
{
}

void UMTDTowerAttackComponent::OnRep_AttackSpeed()
{
}



void UMTDTowerAttackComponent::AttackIntervalCheck()
{
	if (!CurrentTargetEnemy || !OwnerActor->HasAuthority()) return;

	Attack_Server_Reliable();
}

void UMTDTowerAttackComponent::GetAttackRangeDecal()
{
	AttackRangeDecal = OwnerActor->FindComponentByClass<UDecalComponent>();
}

void UMTDTowerAttackComponent::SetAttackRangeDecalValues()
{
	if (!AttackRangeDecal)
	{
		UE_LOG(MTDLog, Error, TEXT("[%s] - Failed to find decal component on owner: on %s"),
			*FString(OwnerActor->HasAuthority() ? TEXT("Server") : TEXT("Client")),
			*GetOwner()->GetName());
		return;
	}

	AttackRangeDecal->SetVisibility(false);
	AttackRangeDecal->DecalSize = FVector(100.0f, AttackRange, AttackRange);
	AttackRangeDecal->SetVisibility(true);
}

void UMTDTowerAttackComponent::FindEnemyToAttack_Server_Reliable_Implementation()
{
	TArray<ABaseEnemy*> EnemiesInRange;

	// Step 1: Gather all enemies within range
	if (BaseTower->GetEntityManager())
	{
		for (ABaseEnemy* Enemy : BaseTower->GetEntityManager()->GetEnemies())
		{
			if (Enemy)
			{
				float Distance = FVector::Dist(BaseTower->GetActorLocation(), Enemy->GetActorLocation());
				if (Distance <= GetAttackRange())
				{
					EnemiesInRange.Add(Enemy);
				}
			}
		}
	}

	// Step 2: Select target based on TargetingMode
	switch (TargetingMode)
	{
	case ETowerTargetingMode::Closest:
	{
		float ClosestDistance = GetAttackRange() * 2;
		ABaseEnemy* ClosestEnemy = nullptr;

		for (ABaseEnemy* Enemy : EnemiesInRange)
		{
			float Distance = FVector::Dist(BaseTower->GetActorLocation(), Enemy->GetActorLocation());
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestEnemy = Enemy;
			}
		}

		CurrentTargetEnemy = ClosestEnemy;
	}
	break;

	case ETowerTargetingMode::FirstTarget:
	{
		float ClosestToGoalDistance = FLT_MAX;
		ABaseEnemy* FirstTargetEnemy = nullptr;

		for (ABaseEnemy* Enemy : EnemiesInRange)
		{
			float DistanceToGoal = Enemy->GetDistanceToGoal();
			if (DistanceToGoal < ClosestToGoalDistance)
			{
				ClosestToGoalDistance = DistanceToGoal;
				FirstTargetEnemy = Enemy;
			}
		}

		CurrentTargetEnemy = FirstTargetEnemy;
	}
	break;

	case ETowerTargetingMode::LastTarget:
	{
		float FurthestToGoalDistance = 0.0f;
		ABaseEnemy* LastTargetEnemy = nullptr;

		for (ABaseEnemy* Enemy : EnemiesInRange)
		{
			float DistanceToGoal = Enemy->GetDistanceToGoal();
			if (DistanceToGoal > FurthestToGoalDistance)
			{
				FurthestToGoalDistance = DistanceToGoal;
				LastTargetEnemy = Enemy;
			}
		}

		CurrentTargetEnemy = LastTargetEnemy;
	}
	break;

	case ETowerTargetingMode::Healthiest:
	{
		float MaxHealth = 0.0f;
		ABaseEnemy* HealthiestEnemy = nullptr;

		for (ABaseEnemy* Enemy : EnemiesInRange)
		{
			float Health = Enemy->GetEnemyHealthComponent()->GetCurrentHealth();
			if (Health > MaxHealth)
			{
				MaxHealth = Health;
				HealthiestEnemy = Enemy;
			}
		}

		CurrentTargetEnemy = HealthiestEnemy;
	}
	break;

	default:
		CurrentTargetEnemy = nullptr;
		break;
	}
}

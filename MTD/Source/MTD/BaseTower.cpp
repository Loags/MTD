// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#include "BaseTower.h"
#include "MTD.h"
#include "TimerManager.h" 
#include "DrawDebugHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "CustomGameState.h"
#include "BaseEnemy.h"
#include "MTDHealthComponent.h"


ABaseTower::ABaseTower()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	AttackDamage = 10.0f;
	AttackRange = 500.0f;
	AttackSpeed = 1.0f;

	EntityManager = nullptr;
	CurrentTargetEnemy = nullptr;
	EnemySearchInterval = 0.2f;

	HealthComponent = CreateDefaultSubobject<UMTDHealthComponent>(TEXT("HealthComponent"));
}

void ABaseTower::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseTower, AttackDamage);
	DOREPLIFETIME(ABaseTower, AttackRange);
	DOREPLIFETIME(ABaseTower, AttackSpeed);
}


void ABaseTower::BeginPlay()
{
	if (HasAuthority())
	{
		if (!InitializeGameState()) return;
		if (!InitializeEntityManager()) return;

		GetWorld()->GetTimerManager().SetTimer(EnemySearchTimerHandle, this, &ABaseTower::FindClosestEnemy_Server_Reliable, EnemySearchInterval, true);
		GetWorld()->GetTimerManager().SetTimer(TowerAttackTimerHandle, this, &ABaseTower::AttackIntervalCheck, AttackSpeed, true);
	}
}

void ABaseTower::Attack_Server_Reliable_Implementation()
{
	if (!HasAuthority() || !CurrentTargetEnemy) return;

	CurrentTargetEnemy->GetHealthComponent()->TakeDamage_Server_Reliable(AttackDamage);
}

bool ABaseTower::Attack_Server_Reliable_Validate()
{
	if (!HasAuthority()) return false;

	// Ensure the tower is not dead and has a valid target within range
	if (HealthComponent->GetIsDead())
	{
		UE_LOG(MTDLog, Warning, TEXT("[%s] - ABaseTower::Attack_Server_Reliable_Validate - Tower is dead: on %s"),
			HasAuthority() ? TEXT("Server") : TEXT("Client"),
			*GetOwner()->GetName());

		return false;
	}

	if (!CurrentTargetEnemy)
	{
		UE_LOG(MTDLog, Warning, TEXT("[%s] - ABaseTower::Attack_Server_Reliable_Validate - Tower has no valid targets: on %s"),
			HasAuthority() ? TEXT("Server") : TEXT("Client"),
			*GetOwner()->GetName());

		return false;
	}

	// Check that the enemy is within range
	float DistanceToTarget = FVector::Dist(GetActorLocation(), CurrentTargetEnemy->GetActorLocation());
	if (DistanceToTarget > GetAttackRange())
	{
		UE_LOG(MTDLog, Warning, TEXT("[%s] - ABaseTower::Attack_Server_Reliable_Validate - Target is out of range: on %s"),
			HasAuthority() ? TEXT("Server") : TEXT("Client"),
			*GetOwner()->GetName());

		return false;
	}

	return true;
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
				UE_LOG(MTDLog, Error, TEXT("[%s] - ABaseTower::FindClosestEnemy_Server_Reliable_Implementation - Encountered a null enemy in CombatEntityManager list: on %s"),
					HasAuthority() ? TEXT("Server") : TEXT("Client"),
					*GetOwner()->GetName());
			}
		}
	}
	else
	{
		UE_LOG(MTDLog, Error, TEXT("[%s] - ABaseTower::FindClosestEnemy_Server_Reliable_Implementation - EntityManager is nullptr: on %s"),
			HasAuthority() ? TEXT("Server") : TEXT("Client"),
			*GetOwner()->GetName());
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

bool ABaseTower::InitializeGameState()
{
	GameState = GetWorld()->GetGameState<ACustomGameState>();
	if (GameState)
	{
		return true;
	}

	UE_LOG(MTDLog, Error, TEXT("[%s] - ABaseTower::InitializeGameState - GameState cast failed: on %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		*GetOwner()->GetName());
	return false;
}

bool ABaseTower::InitializeEntityManager()
{
	EntityManager = GameState->GetEntityManager();
	if (EntityManager)
	{
		EntityManager->RegisterTower(this);
		return true;
	}

	UE_LOG(MTDLog, Error, TEXT("[%s] - ABaseTower::InitializeEntityManager - EntityManager is nullptr: on %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		*GetOwner()->GetName());

	return false;
}

void ABaseTower::AttackIntervalCheck()
{
	if (!CurrentTargetEnemy || !HasAuthority()) return;

	Attack_Server_Reliable();
}

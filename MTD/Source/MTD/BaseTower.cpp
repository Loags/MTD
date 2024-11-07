// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#include "BaseTower.h"
#include "MTD.h"
#include "TimerManager.h" 
#include "DrawDebugHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "CustomGameMode.h"
#include "BaseEnemy.h"


ABaseTower::ABaseTower()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseTower::Tick(float DeltaTime)
{
	ACombatEntity::Tick(DeltaTime);
}


void ABaseTower::BeginPlay()
{
	ACombatEntity::BeginPlay();

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(EnemySearchTimerHandle, this, &ABaseTower::FindClosestEnemy_Implementation, EnemySearchInterval, true);
	}

}

void ABaseTower::FindClosestEnemy_Implementation()
{
	float ClosestDistance = GetAttackRange() * 2;
	ACombatEntity* ClosestEnemy = nullptr;

	// Check if CombatEntityManager is valid
	if (CombatEntityManager)
	{
		UE_LOG(MTDLog, Warning, TEXT("CombatEntityManager is valid. Checking enemies..."));

		// Loop through each enemy in the manager's enemy list
		for (ACombatEntity* Enemy : CombatEntityManager->GetEnemies())
		{
			if (Enemy)
			{
				float Distance = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
				UE_LOG(MTDLog, Warning, TEXT("Enemy found: %s at distance %f"), *Enemy->GetName(), Distance);

				// Check if this enemy is within range and closer than the closest found so far
				if (Distance <= GetAttackRange() && Distance < ClosestDistance)
				{
					UE_LOG(MTDLog, Warning, TEXT("Enemy %s is the closest so far within range."), *Enemy->GetName());
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

	// Update colors based on whether an enemy is found
	if (ClosestEnemy != CurrentTargetEntity)
	{
		UE_LOG(MTDLog, Warning, TEXT("Target has changed. Updating colors..."));

		// Reset color for the previous target if it’s a BaseEnemy
		if (CurrentTargetEntity)
		{
			if (ABaseEnemy* PreviousTarget = Cast<ABaseEnemy>(CurrentTargetEntity))
			{
				UE_LOG(MTDLog, Warning, TEXT("Resetting color for previous target: %s"), *PreviousTarget->GetName());
				PreviousTarget->UpdateEnemyColor_Implementation(FLinearColor::White);
			}
		}

		// Update color for the new target if it’s a BaseEnemy
		if (ClosestEnemy)
		{
			UE_LOG(MTDLog, Warning, TEXT("Set red color!"));

			UpdateTowerHeadColor_Implementation(FLinearColor::Red); // Change Tower color to red
			if (ABaseEnemy* NewTarget = Cast<ABaseEnemy>(ClosestEnemy))
			{
				UE_LOG(MTDLog, Warning, TEXT("Setting new target color to green for enemy: %s"), *NewTarget->GetName());
				NewTarget->UpdateEnemyColor_Implementation(FLinearColor::Green); // Change enemy color to green
			}
			else
			{
				UE_LOG(MTDLog, Warning, TEXT("Closest enemy is not a BaseEnemy. Cannot update color."));
			}
		}
		else
		{
			UE_LOG(MTDLog, Warning, TEXT("No target found within range. Resetting Tower color to white."));
			UE_LOG(MTDLog, Warning, TEXT("Set white color!"));

			UpdateTowerHeadColor_Implementation(FLinearColor::White); // No target, reset tower head to white
		}

		// Update the current target reference
		CurrentTargetEntity = ClosestEnemy;
	}
	else
	{
		UE_LOG(MTDLog, Warning, TEXT("Target remains the same. No color update needed."));
	}
}



// Helper function to update the color of the tower's head
void ABaseTower::UpdateTowerHeadColor_Implementation(const FLinearColor& Color)
{
	if (TowerHead)
	{
		UMaterialInterface* NewMaterial = nullptr;

		// Choose the material based on the input color
		if (Color == FLinearColor::Red)
		{
			NewMaterial = RedMaterial;
		}
		else if (Color == FLinearColor::White)
		{
			NewMaterial = WhiteMaterial;
		}

		// Apply the chosen material if it's valid
		if (NewMaterial)
		{
			TowerHead->SetMaterial(0, NewMaterial);
			UE_LOG(LogTemp, Warning, TEXT("TowerHead material changed based on color input."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No matching material found for the specified color!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No TowerHead found to change material."));
	}

}

void ABaseTower::ServerAttack_Implementation()
{
	if (HasAuthority())
	{
		UE_LOG(MTDLog, Warning, TEXT("Tower is attacking!"));
	}
}

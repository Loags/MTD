// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "CombatEntityManager.h"
#include "BaseEnemy.h"
#include "BaseTower.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/GameplayStatics.h>
#include "MTD.h"


ACombatEntityManager::ACombatEntityManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}
void ACombatEntityManager::LogEntityArrays() const
{
	UE_LOG(MTDLog, Warning, TEXT("Logging CombatEntityManager arrays..."));

	UE_LOG(MTDLog, Warning, TEXT("Enemy Entities:"));
	for (const ACombatEntity* Entity : EnemyEntities)
	{
		UE_LOG(MTDLog, Warning, TEXT("  - %s"), *Entity->GetName());
	}

	UE_LOG(MTDLog, Warning, TEXT("Tower Entities:"));
	for (const ACombatEntity* Entity : TowerEntities)
	{
		UE_LOG(MTDLog, Warning, TEXT("  - %s"), *Entity->GetName());
	}
}

void ACombatEntityManager::RegisterEntity(ACombatEntity* Entity)
{
	if (HasAuthority() && Entity != nullptr)
	{
		UE_LOG(MTDLog, Warning, TEXT("Trying to register object!"))
			if (Entity->IsA(ABaseEnemy::StaticClass()))
			{
				UE_LOG(MTDLog, Warning, TEXT("Add Enemy!"))
					EnemyEntities.Add(Entity);
			}
			else if (Entity->IsA(ABaseTower::StaticClass()))
			{
				UE_LOG(MTDLog, Warning, TEXT("Add Tower!"))
					TowerEntities.Add(Entity);
			}
		LogEntityArrays();
	}
}

void ACombatEntityManager::UnRegisterEntity(ACombatEntity* Entity)
{
	if (HasAuthority() && Entity != nullptr)
	{
		if (Entity->IsA(ABaseEnemy::StaticClass()))
		{
			UE_LOG(MTDLog, Warning, TEXT("Remove Enemy!"))

				EnemyEntities.Remove(Entity);
		}
		else if (Entity->IsA(ABaseTower::StaticClass()))
		{
			UE_LOG(MTDLog, Warning, TEXT("Remove Tower!"))
				TowerEntities.Remove(Entity);
		}
	}
}

void ACombatEntityManager::SpawnEntity_Implementation(TSubclassOf<ACombatEntity> EntityClass, const FVector& Location, const FRotator& Rotation)
{
	ACombatEntity* pSpawnedEntity = GetWorld()->SpawnActor<ACombatEntity>(EntityClass, Location, Rotation);
	if (pSpawnedEntity)
	{
		RegisterEntity(pSpawnedEntity);
	}
}

bool ACombatEntityManager::SpawnEntity_Validate(TSubclassOf<ACombatEntity> EntityClass, const FVector& Location, const FRotator& Rotation)
{
	if (!HasAuthority())
	{
		UE_LOG(MTDLog, Warning, TEXT("SpawnEntity called on a client! Only the server can spawn entities."));
		return false;
	}

	if (EntityClass == nullptr)
	{
		UE_LOG(MTDLog, Warning, TEXT("Invalid EntityClass passed to SpawnEntity."));
		return false;
	}

	return true;
}

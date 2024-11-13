// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "EntityManager.h"
#include <Kismet/GameplayStatics.h>
#include "MTD.h"
#include "Net/UnrealNetwork.h"


AEntityManager::AEntityManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AEntityManager::LogEntityArrays() const
{
	// Log the total number of enemies
	UE_LOG(MTDLog, Log, TEXT("Logging Enemy Entities: Total Enemies = %d"), EnemyEntities.Num());
	for (int32 i = 0; i < EnemyEntities.Num(); ++i)
	{
		if (EnemyEntities[i])
		{
			UE_LOG(MTDLog, Log, TEXT("Enemy[%d]: %s"), i, *EnemyEntities[i]->GetName());
		}
		else
		{
			UE_LOG(MTDLog, Warning, TEXT("Enemy[%d]: Null reference"), i);
		}
	}

	// Log the total number of towers
	UE_LOG(MTDLog, Log, TEXT("Logging Tower Entities: Total Towers = %d"), TowerEntities.Num());
	for (int32 i = 0; i < TowerEntities.Num(); ++i)
	{
		if (TowerEntities[i])
		{
			UE_LOG(MTDLog, Log, TEXT("Tower[%d]: %s"), i, *TowerEntities[i]->GetName());
		}
		else
		{
			UE_LOG(MTDLog, Warning, TEXT("Tower[%d]: Null reference"), i);
		}
	}
}

void AEntityManager::RegisterTower(ABaseTower* Tower)
{
	if (HasAuthority() && Tower != nullptr)
	{
		TowerEntities.Add(Tower);
	}
}

void AEntityManager::RegisterEnemy(ABaseEnemy* Enemy)
{
	if (HasAuthority() && Enemy != nullptr)
	{
		EnemyEntities.Add(Enemy);
	}
}

void AEntityManager::UnRegisterTower(ABaseTower* Tower)
{
	if (HasAuthority() && Tower != nullptr)
	{
		TowerEntities.Remove(Tower);
	}
}

void AEntityManager::UnRegisterEnemy(ABaseEnemy* Enemy)
{
	if (HasAuthority() && Enemy != nullptr)
	{
		EnemyEntities.Remove(Enemy);
	}
}

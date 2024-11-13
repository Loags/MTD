// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "EntityManager.h"
#include <Kismet/GameplayStatics.h>
#include "MTD.h"


AEntityManager::AEntityManager()
{
	PrimaryActorTick.bCanEverTick = false;
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
		UE_LOG(MTDLog, Log, TEXT("*** [Server - Only] *** - Tower (%s) has been added to the EntityManagers list! Total Towers = %d"),
			*FString(Tower->GetName()),
			TowerEntities.Num());
	}
}

void AEntityManager::RegisterEnemy(ABaseEnemy* Enemy)
{
	if (HasAuthority() && Enemy != nullptr)
	{
		EnemyEntities.Add(Enemy);
		UE_LOG(MTDLog, Log, TEXT("*** [Server - Only] *** - Enemy (%s) has been added to the EntityManagers list! Total Enemies = %d"),
			*FString(Enemy->GetName()),
			EnemyEntities.Num());
	}
}

void AEntityManager::UnRegisterTower(ABaseTower* Tower)
{
	if (HasAuthority() && Tower != nullptr)
	{
		TowerEntities.Remove(Tower);
		UE_LOG(MTDLog, Log, TEXT("*** [Server - Only] *** - Tower (%s) has been removed from the EntityManagers list! Total Towers = %d"),
			*FString(Tower->GetName()),
			TowerEntities.Num());
	}
}

void AEntityManager::UnRegisterEnemy(ABaseEnemy* Enemy)
{
	if (HasAuthority() && Enemy != nullptr)
	{
		EnemyEntities.Remove(Enemy);
		UE_LOG(MTDLog, Log, TEXT("*** [Server - Only] *** - Enemy (%s) has been removed from the EntityManagers list! Total Enemies = %d"),
			*FString(Enemy->GetName()),
			EnemyEntities.Num());
	}
}

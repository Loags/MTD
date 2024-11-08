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

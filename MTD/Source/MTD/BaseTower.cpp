// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#include "BaseTower.h"
#include "MTD.h"
#include "DrawDebugHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "CustomGameState.h"
#include "BaseEnemy.h"
#include "MTDHealthComponent.h"
#include "MTDTowerAttackComponent.h"


ABaseTower::ABaseTower()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	EntityManager = nullptr;
	GameState = nullptr;
	HealthComponent = nullptr;
	TowerAttackComponent = nullptr;

	HealthComponent = CreateDefaultSubobject<UMTDHealthComponent>(TEXT("HealthComponent"));
	TowerAttackComponent = CreateDefaultSubobject<UMTDTowerAttackComponent>(TEXT("TowerAttackComponent"));
}

void ABaseTower::BeginPlay()
{
	Super::BeginPlay();
	TowerAttackComponent->SetBaseTower(this);

	if (HasAuthority())
	{
		if (!InitializeGameState()) return;
		if (!InitializeEntityManager()) return;
	}
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
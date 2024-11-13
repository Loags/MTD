// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "BaseEnemy.h"
#include "CustomGameState.h"
#include "MTD.h"
#include "MTDPathFollowingComponent.h"
#include "MTDHealthComponent.h"


ABaseEnemy::ABaseEnemy()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	EntityManager = nullptr;
	AiController = nullptr;
	PathFollowingComponent = nullptr;
	HealthComponent = nullptr;
	GameState = nullptr;

	HealthComponent = CreateDefaultSubobject<UMTDHealthComponent>(TEXT("HealthComponent"));
	PathFollowingComponent = CreateDefaultSubobject<UMTDPathFollowingComponent>(TEXT("PathFollowingComponent"));

	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->InitializeOwner(this);
	HealthComponent->InitializeHealthBar();

	if (HasAuthority())
	{
		if (!InitializeGameState()) return;
		if (!InitializeEntityManager()) return;
		if (!PathFollowingComponent->InitializePath(GameState->GetPathSpline())) return;
		if (!PathFollowingComponent->InitializeAiController(AiController)) return;

		PathFollowingComponent->BindOnMoveCompleted();
		PathFollowingComponent->StartFollowingPath();
	}
}

void ABaseEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AiController = Cast<AAIController>(NewController);

	if (!AiController)
	{
		UE_LOG(MTDLog, Error, TEXT("[%s] - ABaseEnemy::PossessedBy - AIController cast failed: on %s"),
			HasAuthority() ? TEXT("Server") : TEXT("Client"),
			*GetOwner()->GetName());
	}
}

bool ABaseEnemy::InitializeGameState()
{
	GameState = GetWorld()->GetGameState<ACustomGameState>();
	if (GameState)
	{
		return true;
	}

	UE_LOG(MTDLog, Error, TEXT("[%s] - ABaseEnemy::InitializeGameState - CustomGameState cast failed: on %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		*GetOwner()->GetName());
	return false;
}

bool ABaseEnemy::InitializeEntityManager()
{
	EntityManager = GameState->GetEntityManager();
	if (EntityManager)
	{
		EntityManager->RegisterEnemy(this);
		return true;
	}

	UE_LOG(MTDLog, Error, TEXT("[%s] - ABaseEnemy::InitializeEntityManager - EntityManager is nullptr: on %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		*GetOwner()->GetName());
	return false;
}

void ABaseEnemy::Attack_Server_Reliable_Implementation(ABaseTower* Tower)
{
	if (HasAuthority())
	{
	}
}
// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "MTDEnemySpawner.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "MTD.h"
#include "TimerManager.h"
#include "CustomGameState.h"


AMTDEnemySpawner::AMTDEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	GameState = nullptr;

	SpawnInterval = 1.0f;
}

void AMTDEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (!InitializeGameState()) return;
	}
}

void AMTDEnemySpawner::SpawnEnemyAtLocation(const FVector& NewSpawnLocation, const FRotator& NewSpawnRotation)
{
	FVector initSpawnLocation = SpawnLocation;
	FRotator initSpawnRotation = SpawnRotation;

	SpawnLocation = NewSpawnLocation;
	SpawnRotation = NewSpawnRotation;

	SpawnEnemy();

	SpawnLocation = initSpawnLocation;
	SpawnRotation = initSpawnRotation;
}

void AMTDEnemySpawner::SpawnEnemy()
{
	if (EnemyClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);
		GameState->GetRoundManager()->IncrementSpawnedEnemyCount();
	}
	else
	{
		UE_LOG(MTDLog, Error, TEXT("[%s] - AMTDEnemySpawner::SpawnEnemy - EnemyClass is not set: on %s"),
			HasAuthority() ? TEXT("Server") : TEXT("Client"),
			*GetName());

	}
}

void AMTDEnemySpawner::StartSpawnTimer()
{
	if (SpawnInterval > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AMTDEnemySpawner::SpawnEnemy, SpawnInterval, true);
	}
}

void AMTDEnemySpawner::StopSpawnTimer()
{
	if (SpawnTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

bool AMTDEnemySpawner::InitializeGameState()
{
	GameState = GetWorld()->GetGameState<ACustomGameState>();
	if (GameState)
	{
		return true;
	}

	UE_LOG(MTDLog, Error, TEXT("[%s] - AMTDEnemySpawner::InitializeGameState - CustomGameState cast failed: on %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		*GetName());
	return false;
}
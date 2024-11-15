// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "MTDRoundManager.h"
#include "Net/UnrealNetwork.h"
#include "CustomGameState.h"
#include "MTD.h"
#include "MTDEnemySpawner.h"

AMTDRoundManager::AMTDRoundManager()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GameState = nullptr;
	EnemySpawner = nullptr;

	SpawnedEnemyCount = 0;

	DifficultyLevel = 1;
	MaxRoundCount = 1;
	CurrentRoundCount = 0;
	MaxEnemyCount = 10;
	KilledEnemyCount = 0;
	bIsRoundActive = false;
}

void AMTDRoundManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMTDRoundManager, DifficultyLevel);
	DOREPLIFETIME(AMTDRoundManager, MaxRoundCount);
	DOREPLIFETIME(AMTDRoundManager, CurrentRoundCount);
	DOREPLIFETIME(AMTDRoundManager, MaxEnemyCount);
	DOREPLIFETIME(AMTDRoundManager, KilledEnemyCount);
	DOREPLIFETIME(AMTDRoundManager, bIsRoundActive);
}

void AMTDRoundManager::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (!InitializeGameState()) return;
		if (!InitializeEnemySpawner()) return;
		if (!bIsRoundActive)
		{
			StartRound();
		}
	}
}

void AMTDRoundManager::Tick(float DeltaTime)
{
	if (!HasAuthority()) return;

	if (KilledEnemyCount == MaxEnemyCount && bIsRoundActive)
	{
		StopRound();
	}
}

void AMTDRoundManager::IncrementSpawnedEnemyCount()
{
	SpawnedEnemyCount += 1;

	if (SpawnedEnemyCount == MaxEnemyCount)
	{
		EnemySpawner->StopSpawnTimer();
	}
}

bool AMTDRoundManager::InitializeGameState()
{
	GameState = GetWorld()->GetGameState<ACustomGameState>();
	if (GameState)
	{
		return true;
	}

	UE_LOG(MTDLog, Error, TEXT("[%s] - AMTDRoundManager::InitializeGameState - CustomGameState cast failed: on %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		*GetName());
	return false;
}

bool AMTDRoundManager::InitializeEnemySpawner()
{
	EnemySpawner = GameState->GetEnemySpawner();
	if (EnemySpawner)
	{
		return true;
	}

	UE_LOG(MTDLog, Error, TEXT("[%s] - AMTDRoundManager::InitializeEnemySpawner - EnemySpawner is nullptr: on %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		*GetName());
	return false;
}

void AMTDRoundManager::StartRound()
{
	EnemySpawner->StartSpawnTimer();
	bIsRoundActive = true;
	OnRep_bIsRoundActive();

	CurrentRoundCount += 1;
	OnRep_CurrentRoundCount();
}

void AMTDRoundManager::StopRound()
{
	EnemySpawner->StopSpawnTimer();
	bIsRoundActive = false;
	OnRep_bIsRoundActive();
}

void AMTDRoundManager::OnRep_DifficultyLevel()
{
	UE_LOG(MTDLog, Log, TEXT("[%s] - AMTDRoundManager::OnRep_DifficultyLevel - DifficultyLevel: %d on %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		DifficultyLevel,
		*GetName());
}

void AMTDRoundManager::OnRep_MaxRoundCount()
{
	UE_LOG(MTDLog, Log, TEXT("[%s] - AMTDRoundManager::OnRep_MaxRoundCount - Update MaxRoundCount: %d on %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		MaxRoundCount,
		*GetName());
}

void AMTDRoundManager::OnRep_CurrentRoundCount()
{
	UE_LOG(MTDLog, Log, TEXT("[%s] - AMTDRoundManager::OnRep_CurrentRoundCount - Update CurrentRoundCount: %d on %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		CurrentRoundCount,
		*GetName());
}

void AMTDRoundManager::OnRep_MaxEnemyCount()
{
	UE_LOG(MTDLog, Log, TEXT("[%s] - AMTDRoundManager::OnRep_MaxEnemyCount - Update MaxEnemyCount: %d on %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		MaxEnemyCount,
		*GetName());
}

void AMTDRoundManager::OnRep_KilledEnemyCount()
{
	UE_LOG(MTDLog, Log, TEXT("[%s] - AMTDRoundManager::OnRep_KilledEnemyCount - Update KilledEnemyCount: %d on %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		KilledEnemyCount,
		*GetName());
}

void AMTDRoundManager::OnRep_bIsRoundActive()
{
	UE_LOG(MTDLog, Log, TEXT("[%s] - AMTDRoundManager::OnRep_bIsRoundActive - Update bIsRoundActive: %s on %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		*FString(bIsRoundActive ? TEXT("True") : TEXT("False")),
		*GetName());
}

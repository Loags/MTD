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
	MaxRoundCount = 3;
	CurrentRoundCount = 0;
	MaxEnemyCount = 10;
	KilledEnemyCount = 0;
	bIsRoundActive = false;
	InBetweenRoundsInterval = 10.0f;
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

	if (CurrentRoundCount == MaxRoundCount)
	{
		UE_LOG(MTDLog, Error, TEXT("Finished game!"));

		return;
	}

	StartInBetweenRoundsPause();
}

void AMTDRoundManager::StartInBetweenRoundsPause()
{
	if (InBetweenRoundsInterval > 0.0f)
	{
		CountdownSeconds = InBetweenRoundsInterval;

		GetWorld()->GetTimerManager().SetTimer(InBetweenRoundsTimerHandle, this, &AMTDRoundManager::DecreaseCountdown, 1.0f, true);
	}
}

void AMTDRoundManager::DecreaseCountdown()
{
	if (CountdownSeconds > 0)
	{
		CountdownSeconds -= 1;
		OnRep_CountdownSeconds();
	}
	else
	{
		UE_LOG(MTDLog, Log, TEXT("Countdown finished. Ready for the next round!"));

		GetWorld()->GetTimerManager().ClearTimer(InBetweenRoundsTimerHandle);
		PrepareNextRound();
		StartRound();
	}
}

void AMTDRoundManager::PrepareNextRound()
{
	SpawnedEnemyCount = 0;

	MaxEnemyCount += 3;
	OnRep_MaxEnemyCount();

	KilledEnemyCount = 0;
	OnRep_KilledEnemyCount();

	CountdownSeconds = InBetweenRoundsInterval;
	OnRep_CountdownSeconds();
}

void AMTDRoundManager::OnRep_DifficultyLevel()
{
	UE_LOG(MTDLog, Warning, TEXT("[%s] - AMTDRoundManager::OnRep_DifficultyLevel - DifficultyLevel: %d"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		DifficultyLevel);
}

void AMTDRoundManager::OnRep_MaxRoundCount()
{
	UE_LOG(MTDLog, Warning, TEXT("[%s] - AMTDRoundManager::OnRep_MaxRoundCount - Update MaxRoundCount: %d"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		MaxRoundCount);
}

void AMTDRoundManager::OnRep_CurrentRoundCount()
{
	UE_LOG(MTDLog, Warning, TEXT("[%s] - AMTDRoundManager::OnRep_CurrentRoundCount - Update CurrentRoundCount: %d"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		CurrentRoundCount);
}

void AMTDRoundManager::OnRep_MaxEnemyCount()
{
	UE_LOG(MTDLog, Warning, TEXT("[%s] - AMTDRoundManager::OnRep_MaxEnemyCount - Update MaxEnemyCount: %d"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		MaxEnemyCount);
}

void AMTDRoundManager::OnRep_KilledEnemyCount()
{
	UE_LOG(MTDLog, Warning, TEXT("[%s] - AMTDRoundManager::OnRep_KilledEnemyCount - Update KilledEnemyCount: %d"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		KilledEnemyCount);
}

void AMTDRoundManager::OnRep_bIsRoundActive()
{
	UE_LOG(MTDLog, Warning, TEXT("[%s] - AMTDRoundManager::OnRep_bIsRoundActive - Update bIsRoundActive: %s"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		*FString(bIsRoundActive ? TEXT("True") : TEXT("False")));
}

void AMTDRoundManager::OnRep_CountdownSeconds()
{
	UE_LOG(MTDLog, Warning, TEXT("[%s] - AMTDRoundManager::OnRep_CountdownSeconds - Update CountdownSeconds: %f"),
		HasAuthority() ? TEXT("Server") : TEXT("Client"),
		CountdownSeconds);
}

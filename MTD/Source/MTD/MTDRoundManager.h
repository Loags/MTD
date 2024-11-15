// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MTDRoundManager.generated.h"

class ACustomGameState;
class AMTDEnemySpawner;

UCLASS()
class MTD_API AMTDRoundManager : public AActor
{
	GENERATED_BODY()

public:
	AMTDRoundManager();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void IncrementSpawnedEnemyCount();
	inline void IncrementKilledEnemyCount() { KilledEnemyCount += 1; }

private:
	bool InitializeGameState();
	bool InitializeEnemySpawner();
	void StartRound();
	void StopRound();

	UFUNCTION()
	void OnRep_DifficultyLevel();

	UFUNCTION()
	void OnRep_MaxRoundCount();

	UFUNCTION()
	void OnRep_CurrentRoundCount();

	UFUNCTION()
	void OnRep_MaxEnemyCount();

	UFUNCTION()
	void OnRep_KilledEnemyCount();

	UFUNCTION()
	void OnRep_bIsRoundActive();

private:
	ACustomGameState* GameState;
	AMTDEnemySpawner* EnemySpawner;
	int32 SpawnedEnemyCount;

	UPROPERTY(ReplicatedUsing = OnRep_DifficultyLevel)
	int8 DifficultyLevel;

	UPROPERTY(ReplicatedUsing = OnRep_MaxRoundCount)
	int16 MaxRoundCount;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentRoundCount)
	int16 CurrentRoundCount;

	UPROPERTY(ReplicatedUsing = OnRep_MaxEnemyCount)
	int32 MaxEnemyCount;

	UPROPERTY(ReplicatedUsing = OnRep_KilledEnemyCount)
	int32 KilledEnemyCount;

	UPROPERTY(ReplicatedUsing = OnRep_bIsRoundActive)
	bool bIsRoundActive;
};

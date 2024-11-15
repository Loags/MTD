// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "BaseEnemy.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MTDEnemySpawner.generated.h"

class ACustomGameState;

UCLASS()
class MTD_API AMTDEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AMTDEnemySpawner();

protected:
	virtual void BeginPlay() override;

public:
	void SpawnEnemyAtLocation(const FVector& SpawnLocation, const FRotator& SpawnRotation);
	void SpawnEnemy();
	void StartSpawnTimer();
	void StopSpawnTimer();

private:
	bool InitializeGameState();

private:
	ACustomGameState* GameState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	float SpawnInterval;

	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	FRotator SpawnRotation;

};

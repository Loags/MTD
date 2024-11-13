// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "Net/UnrealNetwork.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTower.generated.h"

class AEntityManager;
class ACustomGameState;
class UMTDHealthComponent;
class UMTDTowerAttackComponent;

UCLASS()
class MTD_API ABaseTower : public AActor
{
	GENERATED_BODY()

public:
	ABaseTower();
	inline AEntityManager* GetEntityManager() const { return EntityManager; }
	inline ACustomGameState* GetGameState() const { return GameState; }
	inline UMTDHealthComponent* GetHealthComponent() const { return HealthComponent; }
	inline UMTDTowerAttackComponent* GetTowerAttackComponent() const { return TowerAttackComponent; }

protected:
	virtual void BeginPlay() override;

private:
	bool InitializeGameState();
	bool InitializeEntityManager();

private:
	AEntityManager* EntityManager;
	ACustomGameState* GameState;
	UMTDHealthComponent* HealthComponent;
	UMTDTowerAttackComponent* TowerAttackComponent;
};

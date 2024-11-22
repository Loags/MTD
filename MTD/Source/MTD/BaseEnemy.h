// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "AIController.h"                  
#include "Net/UnrealNetwork.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class AEntityManager;
class ACustomGameState;
class UMTDPathFollowingComponent;
class UMTDEnemyHealthComponent;


UCLASS()
class MTD_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	inline AEntityManager* GetEntityManager() const { return EntityManager; }
	inline AAIController* GetAiController() const { return AiController; }
	inline UMTDPathFollowingComponent* GetPathFollowingComponent() const { return PathFollowingComponent; }
	inline UMTDEnemyHealthComponent* GetEnemyHealthComponent() const { return EnemyHealthComponent; }
	inline ACustomGameState* GetGameState() const { return GameState; }
	float GetDistanceToGoal() const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	virtual void Attack_Server_Reliable(ABaseTower* Target);

private:
	virtual void PossessedBy(AController* NewController) override;
	bool InitializeGameState();
	bool InitializeEntityManager();

protected:
	AEntityManager* EntityManager;
	AAIController* AiController;
	UMTDPathFollowingComponent* PathFollowingComponent;
	UMTDEnemyHealthComponent* EnemyHealthComponent;
	ACustomGameState* GameState;
};

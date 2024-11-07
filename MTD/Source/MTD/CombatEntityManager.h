// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "CombatEntity.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatEntityManager.generated.h"

UCLASS()
class MTD_API ACombatEntityManager : public AActor
{
	GENERATED_BODY()

public:
	ACombatEntityManager();

	void LogEntityArrays() const;

	void RegisterEntity(ACombatEntity* Entity);
	void UnRegisterEntity(ACombatEntity* Entity);

	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnEntity(TSubclassOf<ACombatEntity> EntityClass, const FVector& Location, const FRotator& Rotation);

	const TArray<ACombatEntity*>& GetEnemies() const { return EnemyEntities; }
	const TArray<ACombatEntity*>& GetTowers() const { return EnemyEntities; }

private:
	UPROPERTY(VisibleAnywhere)
	TArray<ACombatEntity*> EnemyEntities;

	UPROPERTY(VisibleAnywhere)
	TArray<ACombatEntity*> TowerEntities;
};

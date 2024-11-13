// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "CoreMinimal.h"
#include "MTDAttackComponent.h"
#include "MTDTowerAttackComponent.generated.h"

class ABaseTower;
class ABaseEnemy;

UCLASS()
class MTD_API UMTDTowerAttackComponent : public UMTDAttackComponent
{
	GENERATED_BODY()

public:
	UMTDTowerAttackComponent();

	inline const void SetBaseTower(ABaseTower* BaseTowerRef) { BaseTower = BaseTowerRef; }
	inline const ABaseEnemy* GetCurrentTarget() const { return CurrentTargetEnemy; }

protected:
	virtual void BeginPlay() override;
	virtual void Attack_Server_Reliable_Implementation() override;
	virtual bool Attack_Server_Reliable_Validate() override;
	virtual void OnRep_AttackDamage() override;
	virtual void OnRep_AttackRange() override;
	virtual void OnRep_AttackSpeed() override;

private:
	UFUNCTION(Server, Reliable)
	void FindClosestEnemy_Server_Reliable();

	UFUNCTION(NetMulticast, Reliable)
	void DrawClosesEnemyLine_Multicast_Reliable(AActor* Actor1, AActor* Actor2, UWorld* World, FColor LineColor, float LineThickness, float Duration);

	virtual void AttackIntervalCheck();

protected:
	float EnemySearchInterval;
	FTimerHandle EnemySearchTimerHandle;
	FTimerHandle TowerAttackTimerHandle;

private:
	ABaseTower* BaseTower;
	ABaseEnemy* CurrentTargetEnemy;
};
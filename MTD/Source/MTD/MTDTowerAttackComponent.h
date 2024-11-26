// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "Components/DecalComponent.h"
#include "CoreMinimal.h"
#include "MTDAttackComponent.h"
#include "MTDTowerAttackComponent.generated.h"

class ABaseTower;
class ABaseEnemy;

UENUM(BlueprintType)
enum class ETowerTargetingMode : uint8
{
	Closest UMETA(DisplayName = "Closest"),
	FirstTarget UMETA(DisplayName = "First Target"),
	LastTarget UMETA(DisplayName = "Last Target"),
	Healthiest UMETA(DisplayName = "Healthiest"),
};

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
	
	UFUNCTION(Server, Reliable)
	void FindEnemyToAttack_Server_Reliable();

	virtual void AttackIntervalCheck();

private:
	void GetAttackRangeDecal();
	void SetAttackRangeDecalValues();

protected:
	float EnemySearchInterval;
	FTimerHandle EnemySearchTimerHandle;
	FTimerHandle TowerAttackTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Targeting")
	ETowerTargetingMode TargetingMode;

	ABaseTower* BaseTower;
	ABaseEnemy* CurrentTargetEnemy;
	UDecalComponent* AttackRangeDecal;
};
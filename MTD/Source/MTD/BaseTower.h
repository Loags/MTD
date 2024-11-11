// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "Net/UnrealNetwork.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTower.generated.h"

class AEntityManager;

UCLASS()
class MTD_API ABaseTower : public AActor
{
	GENERATED_BODY()

public:
	ABaseTower();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	const float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	const float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	const float GetAttackDamage() const { return AttackDamage; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	const float GetAttackRange() const { return AttackRange; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	const float GetAttackSpeed() const { return AttackSpeed; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	virtual void Attack_Server_Reliable(ABaseEnemy* Target);

	UFUNCTION(Server, Reliable)
	virtual void TakeDamage_Server_Reliable(float DamageAmount);

	UFUNCTION(Server, Reliable)
	virtual void Heal_Server_Reliable(float HealAmount);

	UFUNCTION(Server, Reliable)
	void FindClosestEnemy_Server_Reliable();

	UFUNCTION(NetMulticast, Reliable)
	virtual void OnDeath_Multicast_Reliable();

	UFUNCTION(NetMulticast, Reliable)
	void DrawClosesEnemyLine_Multicast_Reliable(AActor* Actor1 = nullptr, AActor* Actor2 = nullptr, UWorld* World = nullptr, FColor LineColor = FColor::Red, float LineThickness = 2.0f, float Duration = -1.0f);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	const ABaseEnemy* GetCurrentTargetEntity() const { return CurrentTargetEnemy; }


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Combat")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Combat")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Combat")
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Combat")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Combat")
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Combat")
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Management")
	AEntityManager* EntityManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ABaseEnemy* CurrentTargetEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float EnemySearchInterval;

	FTimerHandle EnemySearchTimerHandle;
};

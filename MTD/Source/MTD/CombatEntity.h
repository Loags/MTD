// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once


#include "Net/UnrealNetwork.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatEntity.generated.h"

class ACombatEntityManager;


UCLASS()
class MTD_API ACombatEntity : public AActor
{
	GENERATED_BODY()

public:
	ACombatEntity();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	void RegisterManager(ACombatEntityManager* CombatEntityManagerRef);

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerAttack();

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerTakeDamage(float DamageAmount);

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerHeal(float HealAmount);

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastOnDeath();

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


private:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

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

	UPROPERTY(Replicated)
	ACombatEntityManager* CombatEntityManager;
};

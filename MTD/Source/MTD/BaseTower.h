// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de

#pragma once

#include "CombatEntity.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTower.generated.h"



UCLASS()
class MTD_API ABaseTower : public ACombatEntity
{
	GENERATED_BODY()

public:
	ABaseTower();
	virtual void Tick(float DeltaTime) override;

	virtual void ServerAttack_Implementation() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void FindClosestEnemy();

	UFUNCTION(Server, Reliable)
	void UpdateTowerHeadColor(const FLinearColor& Color);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	const ACombatEntity* GetCurrentTargetEntity() const { return CurrentTargetEntity; }


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	ACombatEntity* CurrentTargetEntity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float EnemySearchInterval = 0.2f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* TowerHead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* RedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* WhiteMaterial;

	FTimerHandle EnemySearchTimerHandle;
};

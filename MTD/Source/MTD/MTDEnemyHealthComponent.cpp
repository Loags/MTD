// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "MTDEnemyHealthComponent.h"
#include "BaseEnemy.h"
#include "EntityManager.h"
#include "CustomGameState.h"
#include "MTDRoundManager.h"
#include "MTD.h"

UMTDEnemyHealthComponent::UMTDEnemyHealthComponent()
{

}

void UMTDEnemyHealthComponent::Death_Server_Reliable_Implementation()
{
	UE_LOG(MTDLog, Log, TEXT("[%s] - UMTDEnemyHealthComponent::Death_Server_Reliable_Implementation - Called on %s"),
		*FString(GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client")),
		*GetOwner()->GetName());

	BaseEnemy->GetEntityManager()->UnRegisterEnemy(BaseEnemy);
	BaseEnemy->GetGameState()->GetRoundManager()->IncrementKilledEnemyCount();
	Super::Death_Server_Reliable_Implementation();
}

void UMTDEnemyHealthComponent::OnRep_bIsDead()
{
	Super::OnRep_bIsDead();

	if (!bIsDead) return;

	if (OwnerActor->HasAuthority())
	{
		OwnerActor->Destroy();
	}
}

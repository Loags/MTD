// © 2024+ Loags. All Rights Reserved. This software and its contents, including but not limited to code, assets, artwork, audio, and related files, are the intellectual property of Loags and are protected by copyright law. Unauthorized copying, distribution, modification, or use of any part of this software is strictly prohibited without prior written permission from Loags.Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere. Loags is not affiliated with or endorsed by Epic Games, Inc.For inquiries or licensing information, please contact: Info@Loags.deVisit our website: www.Loags.de


#include "MTDRapidFireTowerAttackComponent.h"
#include "MTD.h"

UMTDRapidFireTowerAttackComponent::UMTDRapidFireTowerAttackComponent()
{
    RapidFireRate = 0.2f; // Fire every X seconds during rapid phase
    CooldownFireRate = 1.0f; // Fire every X second during cooldown phase
    RapidPhaseDuration = 5.0f; // Rapid phase lasts for X seconds
    CooldownPhaseDuration = 3.0f; // Cooldown phase lasts for X seconds

    bIsInRapidFirePhase = true;
    UE_LOG(MTDLog, Log, TEXT("[%s] - URapidFireTowerAttackComponent::Constructor - Initialized Rapid Fire Tower Component"),
        TEXT("Constructor"));
}

void UMTDRapidFireTowerAttackComponent::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(MTDLog, Log, TEXT("URapidFireTowerAttackComponent::BeginPlay"));

    if (OwnerActor->HasAuthority())
    {
        GetWorld()->GetTimerManager().SetTimer(EnemySearchTimerHandle, this, &UMTDRapidFireTowerAttackComponent::FindEnemyToAttack_Server_Reliable, EnemySearchInterval, true);
        GetWorld()->GetTimerManager().SetTimer(TowerAttackTimerHandle, this, &UMTDRapidFireTowerAttackComponent::AttackIntervalCheck, RapidFireRate, true);
        GetWorld()->GetTimerManager().SetTimer(PhaseSwitchTimerHandle, this, &UMTDRapidFireTowerAttackComponent::SwitchFirePhase, RapidPhaseDuration, false);
        UE_LOG(MTDLog, Log, TEXT("[%s] - URapidFireTowerAttackComponent::BeginPlay - Started attack timer with Rapid Fire Rate: %f"),
            *FString(OwnerActor->GetName()), RapidFireRate);

    }
}


void UMTDRapidFireTowerAttackComponent::SwitchFirePhase()
{
    bIsInRapidFirePhase = !bIsInRapidFirePhase;

    float CurrentPhaseDuration = bIsInRapidFirePhase ? RapidPhaseDuration : CooldownPhaseDuration;
    float CurrentFireRate = bIsInRapidFirePhase ? RapidFireRate : CooldownFireRate;

    FString PhaseName = bIsInRapidFirePhase ? TEXT("Rapid Fire Phase") : TEXT("Cooldown Phase");
    UE_LOG(MTDLog, Log, TEXT("[%s] - URapidFireTowerAttackComponent::SwitchFirePhase - Switching to %s, duration: %f seconds"),
        *FString(OwnerActor->GetName()), *PhaseName, CurrentPhaseDuration);

    // Update the attack timer interval based on the current fire rate
    GetWorld()->GetTimerManager().ClearTimer(TowerAttackTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(TowerAttackTimerHandle, this, &UMTDRapidFireTowerAttackComponent::AttackIntervalCheck, CurrentFireRate, true);

    // Set the timer to switch to the next phase after the current phase duration
    GetWorld()->GetTimerManager().SetTimer(PhaseSwitchTimerHandle, this, &UMTDRapidFireTowerAttackComponent::SwitchFirePhase, CurrentPhaseDuration, false);

}

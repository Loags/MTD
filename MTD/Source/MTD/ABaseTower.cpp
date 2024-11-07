// Fill out your copyright notice in the Description page of Project Settings.


#include "ABaseTower.h"

// Sets default values
AABaseTower::AABaseTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AABaseTower::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABaseTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


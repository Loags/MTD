// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(MTDLog, Log, All);

void DrawLineBetweenActors(AActor* Actor1 = nullptr, AActor* Actor2 = nullptr, UWorld* World = nullptr, FColor LineColor = FColor::Red, float LineThickness = 2.0f, float Duration = -1.0f);


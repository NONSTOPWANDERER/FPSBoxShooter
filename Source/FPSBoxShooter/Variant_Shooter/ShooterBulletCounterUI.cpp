// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Shooter/ShooterBulletCounterUI.h"
#include "Components/TextBlock.h"

void UShooterBulletCounterUI::UpdateScore(int32 Score)
{
    UE_LOG(LogTemp, Warning, TEXT("UpdateScore called with: %d"), Score); // 🔍 DEBUG

    if (ScoreText)
    {
        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ScoreText is null!"));
    }
}
// BoxDataTypes.h
#pragma once

#include "CoreMinimal.h"
#include "BoxDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FBoxType
{
    GENERATED_BODY()

    UPROPERTY()
    FString Name;

    UPROPERTY()
    FLinearColor Color;

    UPROPERTY()
    int32 Health;

    UPROPERTY()
    int32 Score;
};

USTRUCT(BlueprintType)
struct FBoxTransform
{
    GENERATED_BODY()

    UPROPERTY()
    FVector Location;

    UPROPERTY()
    FRotator Rotation;

    UPROPERTY()
    FVector Scale;
};

USTRUCT(BlueprintType)
struct FBoxObject
{
    GENERATED_BODY()

    UPROPERTY()
    FString Type;

    UPROPERTY()
    FBoxTransform Transform;
};

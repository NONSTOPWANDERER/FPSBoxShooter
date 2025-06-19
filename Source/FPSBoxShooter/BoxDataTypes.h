#pragma once

#include "CoreMinimal.h"
#include "BoxDataTypes.generated.h"
/**
 * Represents the type definition of a box from the JSON data.
 * Includes its visual and gameplay properties: name, color, health, and score.
 */
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

/**
 * Transform data for spawning a box.
 * Includes location, rotation (in degrees), and scale.
 */
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
/**
 * Represents an individual box instance to be spawned in the world.
 * Refers to a type name (matched with FBoxType) and has its own transform.
 */
USTRUCT(BlueprintType)
struct FBoxObject
{
    GENERATED_BODY()

    UPROPERTY()
    FString Type;

    UPROPERTY()
    FBoxTransform Transform;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxDataTypes.h"
#include "BoxActor.generated.h"
/**
 * ABoxActor represents a damageable box spawned in the world.
 * Its color, health, and score value are initialized from JSON-based data.
 * When shot by a projectile, it takes damage and awards score to the player when destroyed.
 */
UCLASS()
class FPSBOXSHOOTER_API ABoxActor : public AActor
{
    GENERATED_BODY()

public:
    ABoxActor();
    /**
     * Initializes the box properties using data from the fetched JSON.
     * @param BoxData - Struct containing type, color, health, and score info.
     */
    void InitializeBox(const FBoxType& BoxData);
    /**
     * Reduces health when hit. If health reaches 0, awards score and destroys the box.
     * @param Damage - Amount of damage to apply (usually 1 per shot).
     */
    void ApplyDamage(int32 Damage);

private:
    /** Box's visible mesh component (a static cube) */
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BoxMesh;
    /** Base material used to create dynamic colored instances */
    UPROPERTY()
    UMaterialInterface* BaseMaterial;
    /** Current remaining health of the box */
    int32 Health;
    /** Score awarded to the player when this box is destroyed */
    int32 ScoreValue;
};

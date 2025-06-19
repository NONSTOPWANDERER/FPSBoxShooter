
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxDataTypes.h"
#include "BoxActor.generated.h"

UCLASS()
class FPSBOXSHOOTER_API ABoxActor : public AActor
{
    GENERATED_BODY()

public:
    ABoxActor();

    void InitializeBox(const FBoxType& BoxData);
    void ApplyDamage(int32 Damage);

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BoxMesh;

    UPROPERTY()
    UMaterialInterface* BaseMaterial;

    int32 Health;
    int32 ScoreValue;
};

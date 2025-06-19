#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxDataTypes.h"
#include "BoxManager.generated.h"

class UBoxDataFetcher;

UCLASS()
class FPSBOXSHOOTER_API ABoxManager : public AActor
{
    GENERATED_BODY()

public:
    ABoxManager();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UBoxDataFetcher* DataFetcher;

    UFUNCTION()
    void HandleBoxDataReady(const TArray<FBoxType>& Types, const TArray<FBoxObject>& Objects);
};

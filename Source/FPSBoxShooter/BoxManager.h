#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxDataTypes.h"
#include "BoxManager.generated.h"

class UBoxDataFetcher;
/**
 * ABoxManager is responsible for managing the spawning of box actors in the world.
 * It fetches box data from an online JSON source, parses it, and spawns box actors with their respective properties.
 */
UCLASS()
class FPSBOXSHOOTER_API ABoxManager : public AActor
{
    GENERATED_BODY()

public:
    ABoxManager();

protected:
    virtual void BeginPlay() override;

private:
    // Reference to the data fetcher that handles HTTP and JSON parsing
    UPROPERTY()
    UBoxDataFetcher* DataFetcher;
    // Handler function that is called once JSON data is successfully parsed
    // Spawns all box actors based on the parsed data
    UFUNCTION()
    void HandleBoxDataReady(const TArray<FBoxType>& Types, const TArray<FBoxObject>& Objects);
};

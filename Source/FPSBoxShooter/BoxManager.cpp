#include "BoxManager.h"
#include "BoxDataFetcher.h"
#include "BoxActor.h"
#include "Engine/World.h"

ABoxManager::ABoxManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ABoxManager::BeginPlay()
{
    Super::BeginPlay();
    // Create a new instance of the data fetcher to retrieve the JSON box data
    DataFetcher = NewObject<UBoxDataFetcher>(this);
    if (DataFetcher)
    {
        // Bind the delegate to handle the parsed data once it's fetched
        DataFetcher->OnDataReady.AddDynamic(this, &ABoxManager::HandleBoxDataReady);
        // Start fetching the data from the online JSON
        DataFetcher->FetchData();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create BoxDataFetcher instance."));
    }
}

void ABoxManager::HandleBoxDataReady(const TArray<FBoxType>& Types, const TArray<FBoxObject>& Objects)
{
    // Log for debug: how many types and objects we received
	UE_LOG(LogTemp, Warning, TEXT("Box Data Received: %d Types, %d Objects"), Types.Num(), Objects.Num());
    for (const FBoxObject& Obj : Objects)
    {
        // Find the matching type definition for this object
        const FBoxType* MatchedType = Types.FindByPredicate([&](const FBoxType& T) {
            return T.Name == Obj.Type;
        });
        // Skip if no matching type found
        if (!MatchedType) continue;

        FTransform SpawnTransform;
        SpawnTransform.SetLocation(Obj.Transform.Location);
        SpawnTransform.SetRotation(Obj.Transform.Rotation.Quaternion());
        SpawnTransform.SetScale3D(Obj.Transform.Scale);

        ABoxActor* Box = GetWorld()->SpawnActor<ABoxActor>(ABoxActor::StaticClass(), SpawnTransform);
        if (Box)
        {
            Box->InitializeBox(*MatchedType);
        }
    }
}

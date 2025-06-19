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

    DataFetcher = NewObject<UBoxDataFetcher>(this);
    if (DataFetcher)
    {
        // Bind our handler function to the delegate
        DataFetcher->OnDataReady.AddDynamic(this, &ABoxManager::HandleBoxDataReady);
        DataFetcher->FetchData();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create BoxDataFetcher instance."));
    }
}

void ABoxManager::HandleBoxDataReady(const TArray<FBoxType>& Types, const TArray<FBoxObject>& Objects)
{
	UE_LOG(LogTemp, Warning, TEXT("Box Data Received: %d Types, %d Objects"), Types.Num(), Objects.Num());
    for (const FBoxObject& Obj : Objects)
    {
        const FBoxType* MatchedType = Types.FindByPredicate([&](const FBoxType& T) {
            return T.Name == Obj.Type;
        });

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

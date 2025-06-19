#include "BoxDataFetcher.h"

void UBoxDataFetcher::FetchData()
{
    FString URL = TEXT("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json");

    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(URL);
    Request->SetVerb("GET");
    Request->OnProcessRequestComplete().BindUObject(this, &UBoxDataFetcher::OnResponseReceived);
    Request->ProcessRequest();
}

void UBoxDataFetcher::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP request failed"));
        return;
    }

    const FString ResponseStr = Response->GetContentAsString();
    TArray<FBoxType> Types;
    TArray<FBoxObject> Objects;

    if (ParseJson(ResponseStr, Types, Objects))
    {
        OnDataReady.Broadcast(Types, Objects);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON"));
    }
}

bool UBoxDataFetcher::ParseJson(const FString& JsonString, TArray<FBoxType>& OutTypes, TArray<FBoxObject>& OutObjects)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        return false;
    }

    // Parse "types"
    const TArray<TSharedPtr<FJsonValue>>* TypesArray;
    if (JsonObject->TryGetArrayField(TEXT("types"), TypesArray))
    {
        for (auto& TypeVal : *TypesArray)
        {
            TSharedPtr<FJsonObject> TypeObj = TypeVal->AsObject();
            FBoxType Type;
            Type.Name = TypeObj->GetStringField(TEXT("name"));

            const TArray<TSharedPtr<FJsonValue>>& ColorArr = TypeObj->GetArrayField(TEXT("color"));
            Type.Color = FLinearColor(
                ColorArr[0]->AsNumber() / 255.0f,
                ColorArr[1]->AsNumber() / 255.0f,
                ColorArr[2]->AsNumber() / 255.0f,
                1.0f
            );

            Type.Health = TypeObj->GetIntegerField(TEXT("health"));
            Type.Score = TypeObj->GetIntegerField(TEXT("score"));
            OutTypes.Add(Type);
        }
    }

    // Parse "objects"
    const TArray<TSharedPtr<FJsonValue>>* ObjectsArray;
    if (JsonObject->TryGetArrayField(TEXT("objects"), ObjectsArray))
    {
        for (auto& ObjVal : *ObjectsArray)
        {
            TSharedPtr<FJsonObject> Obj = ObjVal->AsObject();
            FBoxObject Object;
            Object.Type = Obj->GetStringField(TEXT("type"));

            const TSharedPtr<FJsonObject> TransformObj = Obj->GetObjectField(TEXT("transform"));

            const TArray<TSharedPtr<FJsonValue>>& Loc = TransformObj->GetArrayField(TEXT("location"));
            Object.Transform.Location = FVector(Loc[0]->AsNumber(), Loc[1]->AsNumber(), Loc[2]->AsNumber());

            const TArray<TSharedPtr<FJsonValue>>& Rot = TransformObj->GetArrayField(TEXT("rotation"));
            Object.Transform.Rotation = FRotator(Rot[1]->AsNumber(), Rot[2]->AsNumber(), Rot[0]->AsNumber()); // pitch, yaw, roll

            const TArray<TSharedPtr<FJsonValue>>& Scale = TransformObj->GetArrayField(TEXT("scale"));
            Object.Transform.Scale = FVector(Scale[0]->AsNumber(), Scale[1]->AsNumber(), Scale[2]->AsNumber());

            OutObjects.Add(Object);
        }
    }

    return true;
}
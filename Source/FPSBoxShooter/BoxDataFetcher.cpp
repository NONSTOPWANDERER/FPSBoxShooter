#include "BoxDataFetcher.h"
/**
 * Initiates an HTTP GET request to fetch the JSON data from the given URL.
 */
void UBoxDataFetcher::FetchData()
{
    // JSON URL containing types and objects
    FString URL = TEXT("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json");
    // Create and configure the HTTP request
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(URL);
    Request->SetVerb("GET");
    // Bind the completion handler
    Request->OnProcessRequestComplete().BindUObject(this, &UBoxDataFetcher::OnResponseReceived);
    // Execute the request
    Request->ProcessRequest();
}
/**
 * Callback for when the HTTP request finishes.
 * Validates the response and attempts to parse it.
 */
void UBoxDataFetcher::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    // Check if the request succeeded
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP request failed"));
        return;
    }
    // Convert the raw JSON to string
    const FString ResponseStr = Response->GetContentAsString();
    TArray<FBoxType> Types;
    TArray<FBoxObject> Objects;
    // Try to parse the JSON
    if (ParseJson(ResponseStr, Types, Objects))
    {
        // Broadcast the parsed data to any listeners
        OnDataReady.Broadcast(Types, Objects);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON"));
    }
}
/**
 * Parses the JSON string and fills arrays of box types and object instances.
 */
bool UBoxDataFetcher::ParseJson(const FString& JsonString, TArray<FBoxType>& OutTypes, TArray<FBoxObject>& OutObjects)
{
    // Convert the string into a JSON object
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        return false;
    }

    /** --- Parse "types" array --- */
    const TArray<TSharedPtr<FJsonValue>>* TypesArray;
    if (JsonObject->TryGetArrayField(TEXT("types"), TypesArray))
    {
        for (auto& TypeVal : *TypesArray)
        {
            TSharedPtr<FJsonObject> TypeObj = TypeVal->AsObject();
            FBoxType Type;
            // Read type name
            Type.Name = TypeObj->GetStringField(TEXT("name"));
            // Read RGB color array and normalize to [0-1]
            const TArray<TSharedPtr<FJsonValue>>& ColorArr = TypeObj->GetArrayField(TEXT("color"));
            Type.Color = FLinearColor(
                ColorArr[0]->AsNumber() / 255.0f,
                ColorArr[1]->AsNumber() / 255.0f,
                ColorArr[2]->AsNumber() / 255.0f,
                1.0f // full alpha
            );
            // Read health and score
            Type.Health = TypeObj->GetIntegerField(TEXT("health"));
            Type.Score = TypeObj->GetIntegerField(TEXT("score"));
            OutTypes.Add(Type);
        }
    }

    /** --- Parse "objects" array --- */
    const TArray<TSharedPtr<FJsonValue>>* ObjectsArray;
    if (JsonObject->TryGetArrayField(TEXT("objects"), ObjectsArray))
    {
        for (auto& ObjVal : *ObjectsArray)
        {
            TSharedPtr<FJsonObject> Obj = ObjVal->AsObject();
            FBoxObject Object;
            // Reference to box type by name
            Object.Type = Obj->GetStringField(TEXT("type"));
            // Get transform object
            const TSharedPtr<FJsonObject> TransformObj = Obj->GetObjectField(TEXT("transform"));
            // Location [x, y, z]
            const TArray<TSharedPtr<FJsonValue>>& Loc = TransformObj->GetArrayField(TEXT("location"));
            Object.Transform.Location = FVector(Loc[0]->AsNumber(), Loc[1]->AsNumber(), Loc[2]->AsNumber());
            // Rotation [roll, pitch, yaw] → note: UE uses pitch, yaw, roll
            const TArray<TSharedPtr<FJsonValue>>& Rot = TransformObj->GetArrayField(TEXT("rotation"));
            Object.Transform.Rotation = FRotator(Rot[1]->AsNumber(), Rot[2]->AsNumber(), Rot[0]->AsNumber()); // pitch, yaw, roll
            // Scale [x, y, z]
            const TArray<TSharedPtr<FJsonValue>>& Scale = TransformObj->GetArrayField(TEXT("scale"));
            Object.Transform.Scale = FVector(Scale[0]->AsNumber(), Scale[1]->AsNumber(), Scale[2]->AsNumber());

            OutObjects.Add(Object);
        }
    }

    return true;
}
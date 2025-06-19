#pragma once

#include "CoreMinimal.h"
#include "BoxDataTypes.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "UObject/NoExportTypes.h"
#include "BoxDataFetcher.generated.h"
/**
 * Delegate to notify when the JSON box data has been successfully fetched and parsed.
 * Sends parsed arrays of Box Types and Box Objects.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoxDataReady, const TArray<FBoxType>&, Types, const TArray<FBoxObject>&, Objects);
/**
 * UBoxDataFetcher is a utility class that performs an HTTP GET request to fetch
 * box configuration data from a JSON file, parses the content, and broadcasts it.
 */
UCLASS()
class FPSBOXSHOOTER_API UBoxDataFetcher : public UObject
{
	GENERATED_BODY()
public:
    /**
     * Initiates the HTTP request to download and parse box data from the configured JSON URL.
     */
    void FetchData();
    /**
     * Event broadcast once the data is successfully fetched and parsed.
     */
    UPROPERTY(BlueprintAssignable)
    FOnBoxDataReady OnDataReady;

private:
    /**
     * Internal handler for when the HTTP response is received.
     * Handles success/failure and triggers parsing.
     */
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    /**
     * Parses the raw JSON string into usable arrays of box types and objects.
     */
    bool ParseJson(const FString& JsonString, TArray<FBoxType>& OutTypes, TArray<FBoxObject>& OutObjects);
};

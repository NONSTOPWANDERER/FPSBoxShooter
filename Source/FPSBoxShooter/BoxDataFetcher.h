// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoxDataTypes.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "UObject/NoExportTypes.h"
#include "BoxDataFetcher.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoxDataReady, const TArray<FBoxType>&, Types, const TArray<FBoxObject>&, Objects);
/**
 * 
 */
UCLASS()
class FPSBOXSHOOTER_API UBoxDataFetcher : public UObject
{
	GENERATED_BODY()
public:
    void FetchData();

    UPROPERTY(BlueprintAssignable)
    FOnBoxDataReady OnDataReady;

private:
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    bool ParseJson(const FString& JsonString, TArray<FBoxType>& OutTypes, TArray<FBoxObject>& OutObjects);
};

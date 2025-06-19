// FPSBoxShooterPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterBulletCounterUI.h"
#include "FPSBoxShooterPlayerController.generated.h"

class UInputMappingContext;

/**
 * Simple first person Player Controller
 * Manages input mappings and score + UI
 */
UCLASS()
class FPSBOXSHOOTER_API AFPSBoxShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	/** Constructor */
	AFPSBoxShooterPlayerController();

	/** Add points to the player score and update UI */
	void AddScore(int32 Points);

	/** Returns current score */
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentScore() const { return PlayerScore; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

protected:
	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Bullet counter UI widget class to create */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UShooterBulletCounterUI> BulletCounterUIClass;

	/** Active UI widget instance */
	UPROPERTY()
	UShooterBulletCounterUI* BulletCounterUI;

private:
	/** Player's current score */
	int32 PlayerScore = 0;
};

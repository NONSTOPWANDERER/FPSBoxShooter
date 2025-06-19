// FPSBoxShooterPlayerController.cpp

#include "FPSBoxShooterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "FPSBoxShooterCameraManager.h"

AFPSBoxShooterPlayerController::AFPSBoxShooterPlayerController()
{
	// Use custom camera manager
	PlayerCameraManagerClass = AFPSBoxShooterCameraManager::StaticClass();
}

void AFPSBoxShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (BulletCounterUIClass)
	{
		BulletCounterUI = CreateWidget<UShooterBulletCounterUI>(this, BulletCounterUIClass);
		if (BulletCounterUI)
		{
			BulletCounterUI->AddToPlayerScreen(0);
			UE_LOG(LogTemp, Log, TEXT("BulletCounterUI created and added."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BulletCounterUI creation failed!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BulletCounterUIClass is not assigned."));
	}
}

void AFPSBoxShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			if (CurrentContext)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}

void AFPSBoxShooterPlayerController::AddScore(int32 Points)
{
	PlayerScore += Points;
	UE_LOG(LogTemp, Log, TEXT("Score updated: %d"), PlayerScore);

	if (BulletCounterUI)
	{
		BulletCounterUI->UpdateScore(PlayerScore);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BulletCounterUI is null – cannot update score display."));
	}
}

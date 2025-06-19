#include "BoxActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "FPSBoxShooterPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ABoxActor::ABoxActor()
{
    PrimaryActorTick.bCanEverTick = false;
    // Create and set root component as a static mesh
    BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
    RootComponent = BoxMesh;
    // Load and assign the default cube mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (BoxMeshAsset.Succeeded())
    {
        BoxMesh->SetStaticMesh(BoxMeshAsset.Object);
    }
     // Load the base material which supports a "Color" parameter
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/BoxShooter/M_BoxMaterial.M_BoxMaterial"));
    if (MaterialAsset.Succeeded())
    {
        BaseMaterial = MaterialAsset.Object;
    }
}
/**
 * Initializes the box’s gameplay properties and sets its color using a dynamic material.
 */
void ABoxActor::InitializeBox(const FBoxType& BoxData)
{
	Health = BoxData.Health;
    ScoreValue = BoxData.Score;
    if (!BoxMesh || !BaseMaterial)
    {
        UE_LOG(LogTemp, Warning, TEXT("BoxMesh or BaseMaterial is null!"));
        return;
    }

    // Create a dynamic material so we can change color at runtime
    UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(BaseMaterial, this);
    if (DynMat)
    {
        // Set the color parameter from the JSON-provided value
        DynMat->SetVectorParameterValue(FName("Color"), BoxData.Color);
        // Apply the dynamic material to the box mesh
        BoxMesh->SetMaterial(0, DynMat);

        UE_LOG(LogTemp, Log, TEXT("Set box color to: %s"), *BoxData.Color.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create dynamic material!"));
    }
}
/**
 * Applies damage to the box. If health falls to 0, the box is destroyed and score is added.
 */
void ABoxActor::ApplyDamage(int32 Damage)
{
    Health -= Damage;

    UE_LOG(LogTemp, Warning, TEXT("Box hit! Remaining Health: %d"), Health);

    if (Health <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Box destroyed. ScoreValue = %d"), ScoreValue);
        // Try to get the player controller to add score
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        UE_LOG(LogTemp, Warning, TEXT("Got Controller: %s"), *GetNameSafe(PC));

        if (PC && PC->IsA(AFPSBoxShooterPlayerController::StaticClass()))
		{
			AFPSBoxShooterPlayerController* ShooterPC = static_cast<AFPSBoxShooterPlayerController*>(PC);
			UE_LOG(LogTemp, Warning, TEXT("Calling AddScore via IsA fallback"));
			ShooterPC->AddScore(ScoreValue);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("IsA check failed for ShooterPlayerController"));
		}
        // Remove the box from the world
        Destroy();
    }
}


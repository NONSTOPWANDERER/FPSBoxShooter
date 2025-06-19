#include "BoxActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "FPSBoxShooterPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ABoxActor::ABoxActor()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
    RootComponent = BoxMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (BoxMeshAsset.Succeeded())
    {
        BoxMesh->SetStaticMesh(BoxMeshAsset.Object);
    }

    // Set default material (will override with dynamic later)
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/BoxShooter/M_BoxMaterial.M_BoxMaterial"));
    if (MaterialAsset.Succeeded())
    {
        BaseMaterial = MaterialAsset.Object;
    }
}

// BoxActor.cpp

void ABoxActor::InitializeBox(const FBoxType& BoxData)
{
	Health = BoxData.Health;
    ScoreValue = BoxData.Score;
    if (!BoxMesh || !BaseMaterial)
    {
        UE_LOG(LogTemp, Warning, TEXT("BoxMesh or BaseMaterial is null!"));
        return;
    }

    // Create dynamic material from M_BoxMaterial
    UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(BaseMaterial, this);
    if (DynMat)
    {
        DynMat->SetVectorParameterValue(FName("Color"), BoxData.Color);
        BoxMesh->SetMaterial(0, DynMat);

        UE_LOG(LogTemp, Log, TEXT("Set box color to: %s"), *BoxData.Color.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create dynamic material!"));
    }
}

void ABoxActor::ApplyDamage(int32 Damage)
{
    Health -= Damage;

    UE_LOG(LogTemp, Warning, TEXT("Box hit! Remaining Health: %d"), Health);

    if (Health <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Box destroyed. ScoreValue = %d"), ScoreValue);

        // 🔍 DEBUG: Check what controller we’re getting
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        UE_LOG(LogTemp, Warning, TEXT("Got Controller: %s"), *GetNameSafe(PC));

        // 🔧 Try casting it to your custom controller
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
        Destroy(); // remove box
    }
}


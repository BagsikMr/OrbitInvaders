#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OrbitGameSettings.h"
#include "OrbitSettingsSubsystem.generated.h"

UCLASS()
class ORBITINVADERS_API UOrbitSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OrbitInvaders|Settings")
	FOrbitGameSettings Settings;

	UFUNCTION(BlueprintCallable, Category = "OrbitInvaders|Settings")
	void ResetToDefaults();
};
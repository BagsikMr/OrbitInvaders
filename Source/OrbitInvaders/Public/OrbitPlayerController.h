
#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "GameFramework/PlayerController.h"
#include "OrbitPlayerController.generated.h"



UCLASS()
class ORBITINVADERS_API AOrbitPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders")
	FKey PauseKey = EKeys::Escape;

private:
	void OnPausePressed();

};
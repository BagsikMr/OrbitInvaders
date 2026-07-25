
#include "OrbitPlayerController.h"
#include "OrbitInvadersGameMode.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

void AOrbitPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		FInputKeyBinding& Binding = InputComponent->BindKey(PauseKey, IE_Pressed, this, &AOrbitPlayerController::OnPausePressed);
		Binding.bExecuteWhenPaused = true;
	}
}

void AOrbitPlayerController::OnPausePressed()
{
	if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->TogglePause();
	}
}

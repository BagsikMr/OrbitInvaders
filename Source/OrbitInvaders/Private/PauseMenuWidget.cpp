
#include "PauseMenuWidget.h"
#include "OrbitInvadersGameMode.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
	}
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnRestartClicked);
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);
	}
}

void UPauseMenuWidget::OnResumeClicked()
{
	if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->ResumeGame();
	}
}

void UPauseMenuWidget::OnRestartClicked()
{
	if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->RestartGame();
	}
}

void UPauseMenuWidget::OnQuitClicked()
{
	if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->QuitGame();
	}
}
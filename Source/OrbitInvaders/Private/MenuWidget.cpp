
#include "MenuWidget.h"
#include "OrbitInvadersGameMode.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMenuWidget::OnStartClicked);
	}
	if (EndlessButton)
	{
		EndlessButton->OnClicked.AddDynamic(this, &UMenuWidget::OnEndlessClicked);
	}
	if (OptionsButton)
	{
		OptionsButton->OnClicked.AddDynamic(this, &UMenuWidget::OnOptionsClicked);
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMenuWidget::OnQuitClicked);
	}
}

void UMenuWidget::OnStartClicked()
{
	if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->StartGame();
	}
}

void UMenuWidget::OnEndlessClicked()
{
	if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->StartEndless();
	}
}

void UMenuWidget::OnOptionsClicked()
{
	if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->ShowOptions();
	}
}

void UMenuWidget::OnQuitClicked()
{
	if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->QuitGame();
	}
}

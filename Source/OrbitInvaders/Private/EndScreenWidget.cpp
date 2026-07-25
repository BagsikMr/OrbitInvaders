
#include "EndScreenWidget.h"
#include "OrbitInvadersGameMode.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UEndScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this));

	if (ResultText)
	{
		const bool bWon = GM && GM->DidPlayerWin();
		ResultText->SetText(FText::FromString(bWon ? TEXT("AMAZE AMAZE AMAZE") : TEXT("GAME OVER\n Rocky fix...")));
	}
	if (ScoreText)
	{
		const int32 Score = GM ? GM->GetScore() : 0;
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
	}

	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UEndScreenWidget::OnRestartClicked);
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UEndScreenWidget::OnQuitClicked);
	}
}

void UEndScreenWidget::OnRestartClicked()
{
	if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->RestartGame();
	}
}

void UEndScreenWidget::OnQuitClicked()
{
	if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->QuitGame();
	}
}

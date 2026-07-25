
#include "HUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Styling/CoreStyle.h"
#include "PlayerShip.h"
#include "Earth.h"
#include "OrbitInvadersGameMode.h"
#include "Kismet/GameplayStatics.h"

AEarth* UHUDWidget::GetEarth()
{
	if (!CachedEarth.IsValid())
	{
		CachedEarth = Cast<AEarth>(UGameplayStatics::GetActorOfClass(this, AEarth::StaticClass()));
	}
	return CachedEarth.Get();
}

void UHUDWidget::BuildHearts(int32 Count)
{
	if (!HeartsBox || !WidgetTree)
	{
		return;
	}

	HeartsBox->ClearChildren();
	Hearts.Reset();

	for (int32 i = 0; i < Count; ++i)
	{
		UWidget* Heart = nullptr;

		if (HeartTexture)
		{
			UImage* Img = WidgetTree->ConstructWidget<UImage>();
			Img->SetBrushFromTexture(HeartTexture);
			Img->SetDesiredSizeOverride(FVector2D(48.f, 48.f));
			Heart = Img;
		}
		else //fallback
		{
			UTextBlock* Txt = WidgetTree->ConstructWidget<UTextBlock>();
			Txt->SetText(FText::FromString(TEXT("\u2665")));
			Txt->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
			Txt->SetFont(FCoreStyle::GetDefaultFontStyle("Regular", 36));
			Heart = Txt;
		}

		if (Heart)
		{
			UPanelSlot* HeartSlot = HeartsBox->AddChild(Heart);
			if (UHorizontalBoxSlot* HSlot = Cast<UHorizontalBoxSlot>(HeartSlot))
			{
				HSlot->SetPadding(FMargin(4.f, 0.f));
			}
			else if (UVerticalBoxSlot* VSlot = Cast<UVerticalBoxSlot>(HeartSlot))
			{
				VSlot->SetPadding(FMargin(0.f, 4.f));
			}
			Hearts.Add(Heart);
		}
	}

	bHeartsBuilt = true;
}

void UHUDWidget::UpdateHearts(int32 CurrentLives)
{
	for (int32 i = 0; i < Hearts.Num(); ++i)
	{
		if (Hearts[i])
		{
			Hearts[i]->SetVisibility(i < CurrentLives ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		}
	}
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const APlayerShip* Ship = Cast<APlayerShip>(GetOwningPlayerPawn());
	const int32 Lives = Ship ? Ship->GetCurrentLives() : 0;
	const int32 Max = Ship ? Ship->GetMaxLives() : 0;

	if (LivesText)
	{
		LivesText->SetText(FText::FromString(FString::Printf(TEXT("Lives: %d / %d"), Lives, Max)));
	}

	if (HeartsBox && Ship)
	{
		if (!bHeartsBuilt)
		{
			BuildHearts(Max);
		}
		UpdateHearts(Lives);
	}

	const AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this));

	if (ScoreText)
	{
		const int32 Score = GM ? GM->GetScore() : 0;
		ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
	}

	if (WaveText)
	{
		if (GM && GM->IsEndless())
		{
			WaveText->SetVisibility(ESlateVisibility::HitTestInvisible);
			WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), GM->GetWave())));
		}
		else
		{
			WaveText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (EarthHpBar || EarthHpText)
	{
		if (const AEarth* Earth = GetEarth())
		{
			if (EarthHpBar)
			{
				EarthHpBar->SetPercent(Earth->GetHealthPercent());
			}
			if (EarthHpText)
			{
				EarthHpText->SetText(FText::FromString(
					FString::Printf(TEXT("Earth: %d / %d"), Earth->GetCurrentHP(), Earth->GetMaxHP())));
			}
		}
	}
}

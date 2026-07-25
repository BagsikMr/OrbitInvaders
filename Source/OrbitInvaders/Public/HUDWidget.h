
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UPanelWidget;
class UWidget;
class UTexture2D;
class AEarth;

UCLASS()
class ORBITINVADERS_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> LivesText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> WaveText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> EarthHpBar;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> EarthHpText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UPanelWidget> HeartsBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TObjectPtr<UTexture2D> HeartTexture;

private:
	TWeakObjectPtr<AEarth> CachedEarth;
	AEarth* GetEarth();

	UPROPERTY()
	TArray<TObjectPtr<UWidget>> Hearts;

	bool bHeartsBuilt = false;

	void BuildHearts(int32 Count);
	void UpdateHearts(int32 CurrentLives);
};

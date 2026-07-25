
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndScreenWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class ORBITINVADERS_API UEndScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ResultText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> QuitButton;

	UFUNCTION()
	void OnRestartClicked();

	UFUNCTION()
	void OnQuitClicked();
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionsWidget.generated.h"

class UButton;
class UWidgetSwitcher;


UCLASS()
class ORBITINVADERS_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> TabSwitcher;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> TabPlayerButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> TabInvadersButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> TabAsteroidsButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> TabSpecialButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> TabEarthButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> TabBunkersButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> ResetButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> BackButton;

	UFUNCTION(BlueprintImplementableEvent, Category = "Options")
	void OnSettingsRefreshed();

	UFUNCTION() void ShowPlayerTab();

	UFUNCTION() void ShowInvadersTab();

	UFUNCTION() void ShowAsteroidsTab();

	UFUNCTION() void ShowSpecialTab();

	UFUNCTION() void ShowEarthTab();

	UFUNCTION() void ShowBunkersTab();

	UFUNCTION() void OnResetClicked();

	UFUNCTION() void OnBackClicked();

private:
	void SetTab(int32 Index);
};
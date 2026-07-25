
#include "OptionsWidget.h"
#include "OrbitInvadersGameMode.h"
#include "OrbitSettingsSubsystem.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TabPlayerButton) { TabPlayerButton->OnClicked.AddDynamic(this, &UOptionsWidget::ShowPlayerTab); }
	if (TabInvadersButton) { TabInvadersButton->OnClicked.AddDynamic(this, &UOptionsWidget::ShowInvadersTab); }
	if (TabAsteroidsButton) { TabAsteroidsButton->OnClicked.AddDynamic(this, &UOptionsWidget::ShowAsteroidsTab); }
	if (TabSpecialButton) { TabSpecialButton->OnClicked.AddDynamic(this, &UOptionsWidget::ShowSpecialTab); }
	if (TabEarthButton) { TabEarthButton->OnClicked.AddDynamic(this, &UOptionsWidget::ShowEarthTab); }
	if (TabBunkersButton) { TabBunkersButton->OnClicked.AddDynamic(this, &UOptionsWidget::ShowBunkersTab); }
	if (ResetButton) { ResetButton->OnClicked.AddDynamic(this, &UOptionsWidget::OnResetClicked); }
	if (BackButton) { BackButton->OnClicked.AddDynamic(this, &UOptionsWidget::OnBackClicked); }

	SetTab(0);
}

void UOptionsWidget::SetTab(int32 Index)
{
	if (TabSwitcher)
	{
		TabSwitcher->SetActiveWidgetIndex(Index);
	}
}

void UOptionsWidget::ShowPlayerTab() { SetTab(0); }
void UOptionsWidget::ShowInvadersTab() { SetTab(1); }
void UOptionsWidget::ShowAsteroidsTab() { SetTab(2); }
void UOptionsWidget::ShowSpecialTab() { SetTab(3); }
void UOptionsWidget::ShowEarthTab() { SetTab(4); }
void UOptionsWidget::ShowBunkersTab() { SetTab(5); }

void UOptionsWidget::OnResetClicked()
{
	if (const UGameInstance* GI = GetGameInstance())
	{
		if (UOrbitSettingsSubsystem* SettingsSys = GI->GetSubsystem<UOrbitSettingsSubsystem>())
		{
			SettingsSys->ResetToDefaults();
		}
	}
	OnSettingsRefreshed();
}

void UOptionsWidget::OnBackClicked()
{
	if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->CloseOptions();
	}
}
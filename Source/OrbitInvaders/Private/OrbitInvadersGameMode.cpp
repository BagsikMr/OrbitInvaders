
#include "OrbitInvadersGameMode.h"
#include "OrbitPlayerController.h"
#include "PlayerShip.h"
#include "InvaderFormation.h"
#include "AsteroidSpawner.h"
#include "Bunker.h"
#include "SpecialInvader.h"
#include "Earth.h"
#include "Engine/GameInstance.h"
#include "OrbitMath.h"
#include "OrbitSettingsSubsystem.h"
#include "HUDWidget.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

AOrbitInvadersGameMode::AOrbitInvadersGameMode() {
	PrimaryActorTick.bCanEverTick = false;
	DefaultPawnClass = APlayerShip::StaticClass();
	PlayerControllerClass = AOrbitPlayerController::StaticClass();
}

void AOrbitInvadersGameMode::BeginPlay() {
	Super::BeginPlay();
	SetupCamera();
	SetupHUD();
	ShowStartMenu();
}

void AOrbitInvadersGameMode::ShowStartMenu()
{
	if (!StartMenuClass)
	{
		StartGame();
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
	{
		StartGame();
		return;
	}

	ActiveMenu = CreateWidget<UUserWidget>(PC, StartMenuClass);
	if (ActiveMenu)
	{
		ActiveMenu->AddToViewport(100);
		UGameplayStatics::SetGamePaused(this, true);
		SetUIInputMode(true);
	}
	else
	{
		StartGame();
	}
}

void AOrbitInvadersGameMode::StartGame()
{
	BeginSession(false);
}

void AOrbitInvadersGameMode::StartEndless()
{
	BeginSession(true);
}

void AOrbitInvadersGameMode::BeginSession(bool bEndless)
{
	if (bGameStarted)
	{
		return;
	}
	bGameStarted = true;
	bEndlessMode = bEndless;
	CurrentWave = 1;

	if (const UOrbitSettingsSubsystem* SettingsSys = GetSettingsSubsystem())
	{
		ActiveSettings = SettingsSys->Settings;
	}

	if (ActiveMenu)
	{
		ActiveMenu->RemoveFromParent();
		ActiveMenu = nullptr;
	}

	UGameplayStatics::SetGamePaused(this, false);
	SetUIInputMode(false);

	ApplyWorldActorConfig();
	SpawnFormation();
	SpawnAsteroidSpawner();
	SpawnBunkers();

	const float Interval = ActiveSettings.Special.SpawnInterval;

	GetWorldTimerManager().SetTimer(SpecialInvaderTimer, this, &AOrbitInvadersGameMode::SpawnSpecialInvader, Interval, true, Interval);
}

UOrbitSettingsSubsystem* AOrbitInvadersGameMode::GetSettingsSubsystem() const
{
	if (const UGameInstance* GI = GetGameInstance())
	{
		return GI->GetSubsystem<UOrbitSettingsSubsystem>();
	}
	return nullptr;
}
void AOrbitInvadersGameMode::ApplyWorldActorConfig()
{
	// Player pawn (auto-spawned by the engine as DefaultPawnClass).
	if (APlayerShip* Ship = Cast<APlayerShip>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		Ship->ApplySettings(ActiveSettings.Player);
		Ship->SetVisual(PlayerVisual);
	}
	// Earth (placed in the level).
	if (AEarth* Earth = Cast<AEarth>(UGameplayStatics::GetActorOfClass(this, AEarth::StaticClass())))
	{
		Earth->ApplySettings(ActiveSettings.Earth);
		Earth->SetVisual(EarthVisual);
	}
}
void AOrbitInvadersGameMode::ShowOptions()
{
	if (!OptionsMenuClass)
	{
		return;
	}
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
	{
		return;
	}
	if (ActiveMenu)
	{
		ActiveMenu->RemoveFromParent();
		ActiveMenu = nullptr;
	}
	ActiveMenu = CreateWidget<UUserWidget>(PC, OptionsMenuClass);
	if (ActiveMenu)
	{
		ActiveMenu->AddToViewport(100);
		SetUIInputMode(true);
	}
	else
	{
		ShowStartMenu();
	}
}
void AOrbitInvadersGameMode::CloseOptions()
{
	if (ActiveMenu)
	{
		ActiveMenu->RemoveFromParent();
		ActiveMenu = nullptr;
	}
	ShowStartMenu();
}

void AOrbitInvadersGameMode::TogglePause()
{
	if (!bGameStarted || bGameEnded)
	{
		return;
	}

	if (bPaused)
	{
		ResumeGame();
		return;
	}

	bPaused = true;
	UGameplayStatics::SetGamePaused(this, true);

	if (PauseMenuClass)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			ActiveMenu = CreateWidget<UUserWidget>(PC, PauseMenuClass);
			if (ActiveMenu)
			{
				ActiveMenu->AddToViewport(100);
			}
		}
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
	}
}

void AOrbitInvadersGameMode::ResumeGame()
{
	if (!bPaused)
	{
		return;
	}
	bPaused = false;

	if (ActiveMenu)
	{
		ActiveMenu->RemoveFromParent();
		ActiveMenu = nullptr;
	}

	UGameplayStatics::SetGamePaused(this, false);
	SetUIInputMode(false);
}

void AOrbitInvadersGameMode::OnWaveCleared()
{
	if (bGameEnded)
	{
		return;
	}

	if (!bEndlessMode)
	{
		EndGame(true);
		return;
	}

	++CurrentWave;

	if (Formation)
	{
		Formation->Destroy();
		Formation = nullptr;
	}

	RefreshBunkers();

	if (Spawner)
	{
		Spawner->ApplyWave(CurrentWave);
	}

	SpawnFormation();
}

void AOrbitInvadersGameMode::RestartGame()
{
	UGameplayStatics::SetGamePaused(this, false);
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	UGameplayStatics::OpenLevel(this, FName(*LevelName));
}

void AOrbitInvadersGameMode::QuitGame()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
}

void AOrbitInvadersGameMode::SetUIInputMode(bool bUIMode)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
	{
		return;
	}
	if (bUIMode)
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->SetShowMouseCursor(true);
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}
}

void AOrbitInvadersGameMode::SpawnFormation()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	UClass* ClassToSpawn = FormationClass.Get();

	if (!ClassToSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("FormationClass not assigned. Using default AInvaderFormation."));
		ClassToSpawn = AInvaderFormation::StaticClass();
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Formation = World->SpawnActor<AInvaderFormation>(ClassToSpawn, FVector::ZeroVector, FRotator::ZeroRotator, Params);
	UE_LOG(LogTemp, Log, TEXT("SpawnFormation (wave %d): %s"), CurrentWave, Formation ? TEXT("spawned") : TEXT("FAILED"));

	if (Formation)
	{
		Formation->ApplySettings(ActiveSettings.Invaders);
		Formation->SetInvaderVisual(InvaderVisual);
		Formation->StartWave(CurrentWave, bEndlessMode);
	}
}

void AOrbitInvadersGameMode::AddScore(int32 Amount)
{
	Score += Amount;
	UE_LOG(LogTemp, Log, TEXT("Score: %d"), Score);
}

void AOrbitInvadersGameMode::EndGame(bool bWon)
{
	if (bGameEnded)
	{
		return;
	}
	bGameEnded = true;
	bPlayerWon = bWon;

	GetWorldTimerManager().ClearTimer(SpecialInvaderTimer);

	UE_LOG(LogTemp, Warning, TEXT("%s (Score: %d)"), bWon ? TEXT("WIN") : TEXT("LOSE"), Score);

	if (EndScreenClass)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			ActiveMenu = CreateWidget<UUserWidget>(PC, EndScreenClass);
			if (ActiveMenu)
			{
				ActiveMenu->AddToViewport(100);
				SetUIInputMode(true);
			}
		}
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, bWon ? FColor::Green : FColor::Red,
			bWon ? TEXT("YOU WIN!") : TEXT("GAME OVER"));
	}

	UGameplayStatics::SetGamePaused(this, true);
}

void AOrbitInvadersGameMode::SetupCamera() {
	UWorld* World = GetWorld();
	
	if (!World) {
		return;
	}

	const FVector CamLocation(0.f, -CameraDistance, 0.f);
	const FRotator CamRotation(0.f, 90.f, 0.f);

	ACameraActor* Cam = World->SpawnActor<ACameraActor>(CamLocation, CamRotation);
	if (!Cam) {
		return;
	}

	if (UCameraComponent* CamComp = Cam->GetCameraComponent())
	{
		CamComp->ProjectionMode = ECameraProjectionMode::Orthographic;
		CamComp->OrthoWidth = OrthoWidth;
		CamComp->SetConstraintAspectRatio(false);
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
	{
		PC->SetViewTarget(Cam);
	}
}

void AOrbitInvadersGameMode::SetupHUD() {
	if (!HUDWidgetClass)
	{
		return;
	}
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		HUDWidget = CreateWidget<UHUDWidget>(PC, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}

void AOrbitInvadersGameMode::SpawnAsteroidSpawner()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	UClass* ClassToSpawn = AsteroidSpawnerClass ? AsteroidSpawnerClass.Get() : AAsteroidSpawner::StaticClass();
	if (!ClassToSpawn)
	{
		return;
	}
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Spawner = World->SpawnActor<AAsteroidSpawner>(ClassToSpawn, FVector::ZeroVector, FRotator::ZeroRotator, Params);

	if (Spawner)
	{
		Spawner->ApplySettings(ActiveSettings.Asteroids);
		Spawner->SetAsteroidVisual(AsteroidVisual);
	}
}

void AOrbitInvadersGameMode::SpawnBunkers()
{
	UWorld* World = GetWorld();

	const int32 Count = ActiveSettings.Bunkers.BunkerCount;

	if (!World || Count <= 0)
	{
		return;
	}

	UClass* ClassToSpawn = BunkerClass ? BunkerClass.Get() : ABunker::StaticClass();

	if (!ClassToSpawn)
	{
		return;
	}

	for (int32 i = 0; i < Count; ++i)
	{
		const float Angle = (360.f / Count) * i;
		const FVector Loc = UOrbitMath::PolarToWorld(BunkerRadius, Angle, 0.f);
		const FRotator Rot = UOrbitMath::FacingOutwardRotation(Angle);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (ABunker* Bunker = World->SpawnActor<ABunker>(ClassToSpawn, Loc, Rot, Params))
		{
			Bunker->ApplySettings(ActiveSettings.Bunkers);
			Bunker->SetVisual(BunkerVisual);
		}
	}
}

void AOrbitInvadersGameMode::SpawnSpecialInvader()
{
	UWorld* World = GetWorld();
	if (!World || bGameEnded)
	{
		return;
	}
	UClass* ClassToSpawn = SpecialInvaderClass ? SpecialInvaderClass.Get() : ASpecialInvader::StaticClass();
	if (!ClassToSpawn)
	{
		return;
	}

	const float StartAngle = FMath::FRandRange(0.f, 360.f);
	const FVector Loc = UOrbitMath::PolarToWorld(SpecialInvaderStartRadius, StartAngle, 0.f);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ASpecialInvader* Special = World->SpawnActor<ASpecialInvader>(ClassToSpawn, Loc, FRotator::ZeroRotator, Params))
	{
		Special->ApplySettings(ActiveSettings.Special);
		Special->SetVisual(SpecialVisual);
		Special->InitSpiral(SpecialInvaderStartRadius, StartAngle);
	}
}

void AOrbitInvadersGameMode::RefreshBunkers()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	TArray<AActor*> Existing;
	UGameplayStatics::GetAllActorsOfClass(World, ABunker::StaticClass(), Existing);
	for (AActor* Actor : Existing)
	{
		if (IsValid(Actor))
		{
			Actor->Destroy();
		}
	}

	SpawnBunkers();
}
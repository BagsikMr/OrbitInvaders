#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OrbitGameSettings.h"
#include "OrbitInvadersGameMode.generated.h"


class UOrbitSettingsSubsystem;

UCLASS()
class ORBITINVADERS_API AOrbitInvadersGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AOrbitInvadersGameMode();

	virtual void BeginPlay() override;

	void AddScore(int32 Amount);

	void EndGame(bool bPlayerWon);

	UFUNCTION(BlueprintCallable, Category = "OrbitInvaders")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "OrbitInvaders")
	void RestartGame();

	UFUNCTION(BlueprintCallable, Category = "OrbitInvaders")
	void QuitGame();

	UFUNCTION(BlueprintCallable, Category = "OrbitInvaders")
	void ShowOptions();

	UFUNCTION(BlueprintCallable, Category = "OrbitInvaders")
	void CloseOptions();

	UFUNCTION(BlueprintCallable, Category = "OrbitInvaders")
	void StartEndless();

	UFUNCTION(BlueprintCallable, Category = "OrbitInvaders")
	void TogglePause();

	UFUNCTION(BlueprintCallable, Category = "OrbitInvaders")
	void ResumeGame();

	bool IsPaused() const { return bPaused; }

	void OnWaveCleared();

	int32 GetScore() const { return Score; }

	bool IsGameOver() const { return bGameEnded; }

	bool DidPlayerWin() const { return bPlayerWon; }

	bool IsEndless() const { return bEndlessMode; }

	int32 GetWave() const { return CurrentWave; }



protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Camera")
	float CameraDistance = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Camera")
	float OrthoWidth = 4600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Waves")
	TSubclassOf<class AInvaderFormation> FormationClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|UI")
	TSubclassOf<class UHUDWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Waves")
	TSubclassOf<class AAsteroidSpawner> AsteroidSpawnerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Bunkers")
	TSubclassOf<class ABunker> BunkerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Bunkers")
	int32 BunkerCount = 6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Bunkers")
	float BunkerRadius = 760.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Special")
	TSubclassOf<class ASpecialInvader> SpecialInvaderClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Special")
	float SpecialInvaderInterval = 12.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Special")
	float SpecialInvaderStartRadius = 1400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|UI")
	TSubclassOf<class UUserWidget> StartMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|UI")
	TSubclassOf<class UUserWidget> EndScreenClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|UI")
	TSubclassOf<class UUserWidget> OptionsMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|UI")
	TSubclassOf<class UUserWidget> PauseMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbitInvaders|Visuals")
	FActorVisual PlayerVisual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbitInvaders|Visuals")
	FActorVisual EarthVisual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbitInvaders|Visuals")
	FActorVisual InvaderVisual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbitInvaders|Visuals")
	FActorVisual SpecialVisual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbitInvaders|Visuals")
	FActorVisual AsteroidVisual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrbitInvaders|Visuals")
	FActorVisual BunkerVisual;

private:
	void SetupCamera();

	void SpawnFormation();

	void SpawnAsteroidSpawner();

	void BeginSession(bool bEndless);

	void RefreshBunkers();

	void SpawnBunkers();

	void SpawnSpecialInvader();

	void SetupHUD();

	void ShowStartMenu();

	void SetUIInputMode(bool bUIMode);

	void ApplyWorldActorConfig();

	UOrbitSettingsSubsystem* GetSettingsSubsystem() const;

	UPROPERTY()
	TObjectPtr<class UHUDWidget> HUDWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> ActiveMenu;

	TObjectPtr<class AInvaderFormation> Formation;

	UPROPERTY()
	TObjectPtr<class AAsteroidSpawner> Spawner;

	FTimerHandle SpecialInvaderTimer;

	FOrbitGameSettings ActiveSettings;

	int32 Score = 0;
	int32 CurrentWave = 1;
	bool bGameEnded = false;
	bool bGameStarted = false;
	bool bPlayerWon = false;
	bool bEndlessMode = false;
	bool bPaused = false;
};

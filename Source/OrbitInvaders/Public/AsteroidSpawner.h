

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asteroid.h"
#include "OrbitGameSettings.h"
#include "AsteroidSpawner.generated.h"

UCLASS()
class ORBITINVADERS_API AAsteroidSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	AAsteroidSpawner();

	virtual void Tick(float DeltaSeconds) override;

	void ApplyWave(int32 WaveNumber);

	void ApplySettings(const FAsteroidSettings& InSettings);

	void SetAsteroidVisual(const FActorVisual& Visual) { AsteroidVisual = Visual; }

protected:
	virtual void BeginPlay() override;

	/** Asteroid to spawn. Falls back to base C++ AAsteroid if unset. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	TSubclassOf<AAsteroid> AsteroidClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float MinSpawnInterval = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float MaxSpawnInterval = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float MinSpeed = 220.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float MaxSpeed = 450.f;

	/** Field half-extents (X, Z) used for spawn edges and wrapping. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	FVector2D HalfBounds = FVector2D(2500.f, 1500.f);

private:
	UPROPERTY()
	FActorVisual AsteroidVisual;

	float TimeToNext = 0.f;

	float BaseMinInterval = 0.f;
	float BaseMaxInterval = 0.f;
	float BaseMinSpeed = 0.f;
	float BaseMaxSpeed = 0.f;

	void SpawnOne();
	float RollInterval() const;
};

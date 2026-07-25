

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbitGameSettings.h"
#include "InvaderFormation.generated.h"

class AEnemyInvader;

USTRUCT()
struct FInvaderSlot
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AEnemyInvader> Invader = nullptr;

	int32 Row = 0;
	float BaseAngleDeg = 0.f;
};

UCLASS()
class ORBITINVADERS_API AInvaderFormation : public AActor
{
	GENERATED_BODY()
	
public:
	AInvaderFormation();

	virtual void Tick(float DeltaTime) override;

	void StartWave(int32 WaveNumber, bool bEndless);

	void ApplySettings(const FInvaderSettings& InSettings);

	void SetInvaderVisual(const FActorVisual& Visual) { InvaderVisual = Visual; }

	void NotifyInvaderDestroyed(AEnemyInvader* Invader);

protected:
	virtual void BeginPlay() override;

	//Formation layout
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Layout")
	TSubclassOf<AEnemyInvader> InvaderClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Layout")
	bool bSurroundEarth = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Layout")
	float RingRowStaggerDeg = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Layout")
	int32 Rows = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Layout")
	int32 Columns = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Layout")
	float CenterAngleDeg = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Layout")
	float ColumnSpacingDeg = 14.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Layout")
	float OuterRadius = 1700.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Layout")
	float RowSpacing = 150.f;

	//Movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Movement")
	float SwingMarginDeg = 18.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Movement")
	float AngularSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Movement")
	float DescendStep = 55.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Movement")
	float LoseRadius = 680.f;

	//Firing
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation|Firing")
	float EnemyFireInterval = 2.2f;

private:
	UPROPERTY()
	TArray<FInvaderSlot> Slots;

	UPROPERTY()
	FActorVisual InvaderVisual;
	
	TArray<float> RowRadius;

	float SwingOffset = 0.f;
	float SwingDir = 1.f;
	float FireTimer = 0.f;
	bool bGameOver = false;

	void SpawnFormation();
	void UpdatePositions();
	void StepInward();
	void TryFire(float DeltaTime);
	void CheckLose();
	int32 CountAlive() const;	
};

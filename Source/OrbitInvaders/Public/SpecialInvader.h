
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HittableInterface.h"
#include "OrbitGameSettings.h"
#include "SpecialInvader.generated.h"

class UStaticMeshComponent;

UCLASS()
class ORBITINVADERS_API ASpecialInvader : public AActor, public IHittableInterface
{
	GENERATED_BODY()

public:
	ASpecialInvader();

	virtual void Tick(float DeltaSeconds) override;

	virtual bool ReceiveProjectileHit(EProjectileTeam ByTeam) override;

	void InitSpiral(float StartRadius, float StartAngleDeg);

	void SetVisual(const FActorVisual& Visual);

	void ApplySettings(const FSpecialSettings& InSettings);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpecialInvader")
	TObjectPtr<UStaticMeshComponent> Mesh;

	// Health points before death
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpecialInvader")
	int32 MaxHP = 3;

	// Angular Speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpecialInvader")
	float AngularSpeed = 100.f;

	// Radial Speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpecialInvader")
	float RadialSpeed = 90.f;

	// Radius for end
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpecialInvader")
	float ReachRadius = 200.f;

	// Dmg for earth if reach it
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpecialInvader")
	int32 EarthDamage = 3;

	// Score points from him to player for being destroyerd
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpecialInvader")
	int32 ScoreValue = 50;

private:
	float CurrentRadius = 1400.f;
	float CurrentAngleDeg = 0.f;
	int32 CurrentHP = 0;
	bool bDead = false;

	void ReachEarth();
};


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HittableInterface.h"
#include "OrbitGameSettings.h"
#include "Earth.generated.h"

class UStaticMeshComponent;

UCLASS()
class ORBITINVADERS_API AEarth : public AActor, public IHittableInterface
{
	GENERATED_BODY()
	
public:	
	AEarth();

	virtual bool ReceiveProjectileHit(EProjectileTeam ByTeam) override;

	void ApplyDamage(int32 Amount);

	void SetVisual(const FActorVisual& Visual);

	void ApplySettings(const FEarthSettings& InSettings);

	int32 GetCurrentHP() const { return CurrentHP; }
	int32 GetMaxHP() const { return MaxHP; }
	float GetHealthPercent() const { return MaxHP > 0 ? (float)CurrentHP / (float)MaxHP : 0.f; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Earth")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Earth")
	float Radius = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Earth")
	int32 MaxHP = 10;

private:
	int32 CurrentHP = 0;

};

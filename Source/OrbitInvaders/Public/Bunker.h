
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HittableInterface.h"
#include "OrbitGameSettings.h"
#include "Bunker.generated.h"

class UStaticMeshComponent;

UCLASS()
class ORBITINVADERS_API ABunker : public AActor, public IHittableInterface
{
	GENERATED_BODY()

public:
	ABunker();

	virtual bool ReceiveProjectileHit(EProjectileTeam ByTeam) override;

	void SetVisual(const FActorVisual& Visual);

	void ApplySettings(const FBunkerSettings& InSettings);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bunker")
	TObjectPtr<UStaticMeshComponent> Mesh;

	// Bunker hp before "death"
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bunker")
	int32 MaxHP = 6;

private:
	int32 CurrentHP = 0;
	FVector BaseScale = FVector(2.f, 0.6f, 0.6f);

	void UpdateDamageVisual();
};

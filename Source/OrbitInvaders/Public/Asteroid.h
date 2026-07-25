

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HittableInterface.h"
#include "OrbitGameSettings.h"
#include "Asteroid.generated.h"

class UStaticMeshComponent;
class UPrimitiveComponent;
struct FHitResult;

UENUM(BlueprintType)
enum class EAsteroidSize : uint8
{
	Small,
	Medium,
	Big
};

UCLASS()
class ORBITINVADERS_API AAsteroid : public AActor, public IHittableInterface
{
	GENERATED_BODY()
	
public:
	AAsteroid();

	virtual void Tick(float DeltaSeconds) override;

	virtual bool ReceiveProjectileHit(EProjectileTeam ByTeam) override;

	void InitAsteroid(EAsteroidSize InSize, const FVector& InVelocity, const FVector2D& InHalfBounds);

	void SetVisual(const FActorVisual& Visual);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asteroid")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asteroid")
	EAsteroidSize Size = EAsteroidSize::Big;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asteroid|Visual")
	float BigScale = 6.4f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asteroid|Visual")
	float MediumScale = 3.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asteroid|Visual")
	float SmallScale = 1.6f;

	/** Child fragments fly a bit faster than the parent. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asteroid|Split")
	float ChildSpeedMultiplier = 1.15f;

	/** Half-angle (deg) the two fragments spread from the parent's direction. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asteroid|Split")
	float SplitSpreadDeg = 25.f;

private:
	FVector Velocity = FVector::ZeroVector;
	FVector2D HalfBounds = FVector2D(2500.f, 1500.f);
	bool bDead = false;

	UPROPERTY()
	FActorVisual VisualOverride;

	UFUNCTION()
	void OnEarthOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ApplySizeVisual();
	void WrapPosition();
	void Split();
};

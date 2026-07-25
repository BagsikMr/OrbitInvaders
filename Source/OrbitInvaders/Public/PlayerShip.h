

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HittableInterface.h"
#include "OrbitGameSettings.h"
#include "PlayerShip.generated.h"


class UStaticMeshComponent;
class UInputMappingContext;
class UInputAction;
class AProjectile;
struct FInputActionValue;

UCLASS()
class ORBITINVADERS_API APlayerShip : public APawn, public IHittableInterface
{
	GENERATED_BODY()

public:
	APlayerShip();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void SetVisual(const FActorVisual& Visual);

	void ApplySettings(const FPlayerSettings& InSettings);

	virtual bool ReceiveProjectileHit(EProjectileTeam ByTeam) override;

	int32 GetCurrentLives() const { return CurrentLives; }
	int32 GetMaxLives() const { return MaxLives; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Orbit")
	float OrbitRadius = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Orbit")
	float AngularSpeed = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Orbit")
	float StartAngleDeg = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Input")
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Combat")
	float MuzzleOffset = 70.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Combat")
	float FireCooldown = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Health")
	int32 MaxLives = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OrbitInvaders|Health")
	float InvulnAfterHit = 0.8f;

private:
	float CurrentAngleDeg = 90.f;

	float MoveInput = 0.f;

	float LastFireTime = -1000.f;

	int32 CurrentLives = 0;

	float LastHitTime = -1000.f;

	void UpdateOrbitTransform();

	void OnMove(const FInputActionValue& Value);
	void OnMoveReleased(const FInputActionValue& Value);
	void OnFire(const FInputActionValue& Value);

	
};



#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UENUM(BlueprintType)
enum class EProjectileTeam : uint8
{
	Player,
	Enemy
};

UCLASS()
class ORBITINVADERS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:

	AProjectile();

	void InitProjectile(const FVector& Direction, EProjectileTeam InTeam);

	EProjectileTeam GetTeam() const { return Team; }

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float Speed = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float LifeSeconds = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	EProjectileTeam Team = EProjectileTeam::Player;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep);
	
};

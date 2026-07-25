

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HittableInterface.h"
#include "Projectile.h"
#include "OrbitGameSettings.h"
#include "EnemyInvader.generated.h"

class UStaticMeshComponent;
class AInvaderFormation;
class AProjectile;

UCLASS()
class ORBITINVADERS_API AEnemyInvader : public AActor, public IHittableInterface
{
	GENERATED_BODY()
	
public:	
	AEnemyInvader();

	virtual bool ReceiveProjectileHit(EProjectileTeam ByTeam) override;

	void SetFormation(AInvaderFormation* InFormation) { OwningFormation = InFormation; }

	void FireAtEarth();

	void SetVisual(const FActorVisual& Visual);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float MuzzleOffset = 45.f;

private:
	TWeakObjectPtr<AInvaderFormation> OwningFormation;
	bool bDead = false;
};

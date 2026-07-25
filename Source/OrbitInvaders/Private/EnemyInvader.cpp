


#include "EnemyInvader.h"
#include "InvaderFormation.h"
#include "OrbitMath.h"
#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"


AEnemyInvader::AEnemyInvader()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		Mesh->SetStaticMesh(CubeMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BasicMat(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (BasicMat.Succeeded())
	{
		Mesh->SetMaterial(0, BasicMat.Object);
	}
	Mesh->SetRelativeScale3D(FVector(0.6f));

	Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Mesh->SetGenerateOverlapEvents(true);

}

void AEnemyInvader::SetVisual(const FActorVisual& Visual)
{
	OrbitVisual::Apply(Mesh, Visual);
}

bool AEnemyInvader::ReceiveProjectileHit(EProjectileTeam ByTeam)
{
	if (ByTeam != EProjectileTeam::Player)
	{
		return false;
	}
	if (bDead) 
	{
		return true;
	}
	bDead = true;

	if (OwningFormation.IsValid())
	{
		OwningFormation->NotifyInvaderDestroyed(this);
	}
	Destroy();
	return true;
}

void AEnemyInvader::FireAtEarth()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	float Radius, AngleDeg;
	UOrbitMath::WorldToPolar(GetActorLocation(), Radius, AngleDeg);

	const FVector Dir = -UOrbitMath::RadialDirection(AngleDeg);
	const FVector SpawnLoc = GetActorLocation() + Dir * MuzzleOffset;

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = nullptr;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UClass* ClassToSpawn = ProjectileClass.Get();

	if (!ClassToSpawn)
	{
		ClassToSpawn = AProjectile::StaticClass();
	}

	if (AProjectile* Proj = World->SpawnActor<AProjectile>(ClassToSpawn, SpawnLoc, Dir.Rotation(), Params))
	{
		Proj->InitProjectile(Dir, EProjectileTeam::Enemy);
	}
}



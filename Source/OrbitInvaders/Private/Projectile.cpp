


#include "Projectile.h"
#include "HittableInterface.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"


AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(16.f);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = Collision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		Mesh->SetStaticMesh(SphereMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BasicMat(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (BasicMat.Succeeded())
	{
		Mesh->SetMaterial(0, BasicMat.Object);
	}
	Mesh->SetRelativeScale3D(FVector(0.3f));

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->ProjectileGravityScale = 0.f;
	Movement->bRotationFollowsVelocity = true;
	Movement->InitialSpeed = Speed;
	Movement->MaxSpeed = Speed;
	Movement->bConstrainToPlane = true;
	Movement->SetPlaneConstraintNormal(FVector(0.f, 1.f, 0.f));
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSeconds);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
	
}

void AProjectile::InitProjectile(const FVector& Direction, EProjectileTeam InTeam)
{
	Team = InTeam;
	const FVector Dir = Direction.GetSafeNormal();
	if (Movement)
	{
		Movement->Velocity = Dir * Speed;
	}
	SetActorRotation(Dir.Rotation());
}

void AProjectile::OnOverlap(UPrimitiveComponent* /*OverlappedComp*/, AActor* OtherActor,
	UPrimitiveComponent* /*OtherComp*/, int32 /*OtherBodyIndex*/, bool /*bFromSweep*/, const FHitResult& /*Sweep*/)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner() || OtherActor == GetInstigator())
	{
		return;
	}

	if (OtherActor->IsA(AProjectile::StaticClass()))
	{
		return;
	}

	if (IHittableInterface* Hittable = Cast<IHittableInterface>(OtherActor))
	{
		if (Hittable->ReceiveProjectileHit(Team))
		{
			Destroy();
		}
		return;
	}

	Destroy();
}



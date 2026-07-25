

#include "Asteroid.h"
#include "Earth.h"
#include "OrbitInvadersGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

namespace
{
	FVector RotateInXZ(const FVector& V, float AngleDeg)
	{
		const float Rad = FMath::DegreesToRadians(AngleDeg);
		const float C = FMath::Cos(Rad);
		const float S = FMath::Sin(Rad);
		return FVector(V.X * C - V.Z * S, V.Y, V.X * S + V.Z * C);
	}
}

AAsteroid::AAsteroid()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

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

	Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAsteroid::OnEarthOverlap);
}

void AAsteroid::OnEarthOverlap(UPrimitiveComponent* /*OverlappedComp*/, AActor* OtherActor, UPrimitiveComponent* /*OtherComp*/,
	int32 /*OtherBodyIndex*/, bool /*bFromSweep*/, const FHitResult& /*SweepResult*/)
{
	if (bDead || !OtherActor)
	{
		return;
	}

	if (OtherActor->IsA(AEarth::StaticClass()))
	{
		bDead = true;
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->EndGame(false);
		}
		Destroy();
	}
}

void AAsteroid::InitAsteroid(EAsteroidSize InSize, const FVector& InVelocity, const FVector2D& InHalfBounds)
{
	Size = InSize;
	Velocity = InVelocity;
	HalfBounds = InHalfBounds;
	ApplySizeVisual();
}

void AAsteroid::SetVisual(const FActorVisual& Visual)
{
	VisualOverride = Visual;
	OrbitVisual::Apply(Mesh, Visual, /*bApplyScale=*/false);
}

void AAsteroid::ApplySizeVisual()
{
	float Scale = MediumScale;
	switch (Size)
	{
	case EAsteroidSize::Big:    Scale = BigScale; break;
	case EAsteroidSize::Medium: Scale = MediumScale; break;
	case EAsteroidSize::Small:  Scale = SmallScale; break;
	}
	Mesh->SetRelativeScale3D(FVector(Scale));
}

void AAsteroid::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AddActorWorldOffset(Velocity * DeltaSeconds);
	WrapPosition();
}

void AAsteroid::WrapPosition()
{
	FVector Loc = GetActorLocation();
	const float W = 2.f * HalfBounds.X;
	const float H = 2.f * HalfBounds.Y;

	if (Loc.X > HalfBounds.X) { Loc.X -= W; }
	else if (Loc.X < -HalfBounds.X) { Loc.X += W; }

	if (Loc.Z > HalfBounds.Y) { Loc.Z -= H; }
	else if (Loc.Z < -HalfBounds.Y) { Loc.Z += H; }

	SetActorLocation(Loc);
}

bool AAsteroid::ReceiveProjectileHit(EProjectileTeam ByTeam)
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
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->AddScore(5);
	}

	if (Size != EAsteroidSize::Small)
	{
		Split();
	}

	Destroy();
	return true;
}

void AAsteroid::Split()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const EAsteroidSize ChildSize = (Size == EAsteroidSize::Big) ? EAsteroidSize::Medium : EAsteroidSize::Small;

	FVector BaseDir = Velocity.GetSafeNormal();
	if (BaseDir.IsNearlyZero())
	{
		BaseDir = FVector(1.f, 0.f, 0.f);
	}
	const float ChildSpeed = Velocity.Size() * ChildSpeedMultiplier;

	const float Angles[2] = { +SplitSpreadDeg, -SplitSpreadDeg };
	for (float Angle : Angles)
	{
		const FVector Dir = RotateInXZ(BaseDir, Angle);
		const FVector ChildVel = Dir * ChildSpeed;
		const FVector SpawnLoc = GetActorLocation() + Dir * 30.f; // offset so it doesn't re-hit the same bullet

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (AAsteroid* Child = World->SpawnActor<AAsteroid>(GetClass(), SpawnLoc, FRotator::ZeroRotator, Params))
		{
			Child->SetVisual(VisualOverride);
			Child->InitAsteroid(ChildSize, ChildVel, HalfBounds);
		}
	}
}



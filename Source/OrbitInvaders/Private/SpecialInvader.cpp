
#include "SpecialInvader.h"
#include "Earth.h"
#include "OrbitMath.h"
#include "OrbitInvadersGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ASpecialInvader::ASpecialInvader()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (ConeMesh.Succeeded())
	{
		Mesh->SetStaticMesh(ConeMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BasicMat(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (BasicMat.Succeeded())
	{
		Mesh->SetMaterial(0, BasicMat.Object);
	}
	Mesh->SetRelativeScale3D(FVector(0.8f));

	Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Mesh->SetGenerateOverlapEvents(true);
}

void ASpecialInvader::SetVisual(const FActorVisual& Visual)
{
	OrbitVisual::Apply(Mesh, Visual);
}
void ASpecialInvader::ApplySettings(const FSpecialSettings& InSettings)
{
	MaxHP = InSettings.MaxHP;
	AngularSpeed = InSettings.AngularSpeed;
	RadialSpeed = InSettings.RadialSpeed;
	EarthDamage = InSettings.EarthDamage;
}

void ASpecialInvader::InitSpiral(float StartRadius, float StartAngleDeg)
{
	CurrentRadius = StartRadius;
	CurrentAngleDeg = StartAngleDeg;
	CurrentHP = MaxHP;
	SetActorLocation(UOrbitMath::PolarToWorld(CurrentRadius, CurrentAngleDeg, 0.f));
}

void ASpecialInvader::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bDead)
	{
		return;
	}

	CurrentAngleDeg = UOrbitMath::WrapAngle(CurrentAngleDeg + AngularSpeed * DeltaSeconds);
	CurrentRadius -= RadialSpeed * DeltaSeconds;

	if (CurrentRadius <= ReachRadius)
	{
		ReachEarth();
		return;
	}

	SetActorLocation(UOrbitMath::PolarToWorld(CurrentRadius, CurrentAngleDeg, 0.f));
	SetActorRotation(UOrbitMath::FacingOutwardRotation(CurrentAngleDeg));
}

void ASpecialInvader::ReachEarth()
{
	bDead = true;
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (AEarth* Earth = Cast<AEarth>(UGameplayStatics::GetActorOfClass(this, AEarth::StaticClass())))
	{
		Earth->ApplyDamage(EarthDamage);
	}
	Destroy();
}

bool ASpecialInvader::ReceiveProjectileHit(EProjectileTeam ByTeam)
{
	if (ByTeam != EProjectileTeam::Player)
	{
		return false;
	}
	if (bDead)
	{
		return true;
	}

	CurrentHP = FMath::Max(0, CurrentHP - 1);
	if (CurrentHP <= 0)
	{
		bDead = true;
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->AddScore(ScoreValue);
		}
		Destroy();
	}
	return true;
}

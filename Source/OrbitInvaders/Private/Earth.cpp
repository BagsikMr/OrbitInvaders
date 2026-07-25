


#include "Earth.h"
#include "OrbitInvadersGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


AEarth::AEarth()
{
	PrimaryActorTick.bCanEverTick = false;

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

	Mesh->SetRelativeScale3D(FVector(Radius / 50.f));

	Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Mesh->SetGenerateOverlapEvents(true);
}

void AEarth::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = MaxHP;
}

void AEarth::SetVisual(const FActorVisual& Visual)
{
	OrbitVisual::Apply(Mesh, Visual);
}
void AEarth::ApplySettings(const FEarthSettings& InSettings)
{
	MaxHP = InSettings.MaxHP;

	CurrentHP = MaxHP;
}

bool AEarth::ReceiveProjectileHit(EProjectileTeam ByTeam)
{
	if (ByTeam != EProjectileTeam::Enemy)
	{
		return false;
	}
	ApplyDamage(1);
	return true;
}

void AEarth::ApplyDamage(int32 Amount)
{
	if (CurrentHP <= 0)
	{
		return;
	}
	CurrentHP = FMath::Max(0, CurrentHP - Amount);
	UE_LOG(LogTemp, Log, TEXT("Earth HP = %d / %d"), CurrentHP, MaxHP);

	if (CurrentHP <= 0)
	{
		if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->EndGame(false);
		}
	}
}

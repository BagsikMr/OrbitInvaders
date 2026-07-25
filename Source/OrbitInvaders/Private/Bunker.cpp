
#include "Bunker.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ABunker::ABunker()
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

	Mesh->SetRelativeScale3D(BaseScale);

	Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Mesh->SetGenerateOverlapEvents(true);
}

void ABunker::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = MaxHP;
	UpdateDamageVisual();
}

void ABunker::SetVisual(const FActorVisual& Visual)
{
	OrbitVisual::Apply(Mesh, Visual, /*bApplyScale=*/false);
}
void ABunker::ApplySettings(const FBunkerSettings& InSettings)
{
	MaxHP = InSettings.MaxHP;
	CurrentHP = MaxHP;
	UpdateDamageVisual();
}

bool ABunker::ReceiveProjectileHit(EProjectileTeam /*ByTeam*/)
{
	if (CurrentHP <= 0)
	{
		return true;
	}

	CurrentHP = FMath::Max(0, CurrentHP - 1);
	if (CurrentHP <= 0)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Destroy();
	}
	else
	{
		UpdateDamageVisual();
	}
	return true;
}

void ABunker::UpdateDamageVisual()
{
	const float HealthFrac = MaxHP > 0 ? (float)CurrentHP / (float)MaxHP : 0.f;
	const float Shrink = FMath::Lerp(0.4f, 1.f, HealthFrac);
	Mesh->SetRelativeScale3D(FVector(BaseScale.X, BaseScale.Y, BaseScale.Z * Shrink));
}

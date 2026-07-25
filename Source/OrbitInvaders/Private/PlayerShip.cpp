

#include "PlayerShip.h"
#include "OrbitMath.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "Projectile.h"
#include "OrbitInvadersGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

APlayerShip::APlayerShip()
{

	PrimaryActorTick.bCanEverTick = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMesh(TEXT("/Engine/BasicShapes/Cone.Cone"));
	if (ConeMesh.Succeeded())
	{
		Mesh->SetStaticMesh(ConeMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BasicMat(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (BasicMat.Succeeded())
	{
		Mesh->SetMaterial(0, BasicMat.Object);
	}
	Mesh->SetRelativeScale3D(FVector(0.6f));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionProfileName(TEXT("Pawn"));
}

void APlayerShip::SetVisual(const FActorVisual& Visual)
{
	OrbitVisual::Apply(Mesh, Visual);
}
void APlayerShip::ApplySettings(const FPlayerSettings& InSettings)
{
	AngularSpeed = InSettings.AngularSpeed;
	FireCooldown = InSettings.FireCooldown;
	MaxLives = InSettings.MaxLives;
	InvulnAfterHit = InSettings.InvulnAfterHit;
	CurrentLives = MaxLives;
}

void APlayerShip::BeginPlay()
{
	Super::BeginPlay();

	CurrentAngleDeg = StartAngleDeg;
	CurrentLives = MaxLives;
	UpdateOrbitTransform();

	if (const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (MappingContext)
			{
				Subsystem->AddMappingContext(MappingContext, 0);
			}
		}
	}
	
}

void APlayerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!FMath::IsNearlyZero(MoveInput))
	{
		CurrentAngleDeg = UOrbitMath::WrapAngle(CurrentAngleDeg + MoveInput * AngularSpeed * DeltaTime);
		UpdateOrbitTransform();
	}

}

void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerShip::OnMove);
			EIC->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerShip::OnMoveReleased);
			EIC->BindAction(MoveAction, ETriggerEvent::Canceled, this, &APlayerShip::OnMoveReleased);
		}
		if (FireAction)
		{
			EIC->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerShip::OnFire);
		}
	}
}

void APlayerShip::UpdateOrbitTransform()
{
	SetActorLocation(UOrbitMath::PolarToWorld(OrbitRadius, CurrentAngleDeg, 0.f));
	SetActorRotation(UOrbitMath::FacingOutwardRotation(CurrentAngleDeg));
}

void APlayerShip::OnMove(const FInputActionValue& Value)
{
	MoveInput = Value.Get<float>();
}

void APlayerShip::OnMoveReleased(const FInputActionValue& /*Value*/)
{
	MoveInput = 0.f;
}

bool APlayerShip::ReceiveProjectileHit(EProjectileTeam ByTeam)
{
	if (ByTeam != EProjectileTeam::Enemy)
	{
		return false;
	}

	UWorld* World = GetWorld();
	const float Now = World ? World->GetTimeSeconds() : 0.f;

	if (Now - LastHitTime < InvulnAfterHit)
	{
		return true;
	}
	LastHitTime = Now;

	CurrentLives = FMath::Max(0, CurrentLives - 1);
	UE_LOG(LogTemp, Log, TEXT("PlayerShip hit! Lives = %d"), CurrentLives);

	if (CurrentLives <= 0)
	{
		if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->EndGame(false);
		}
	}
	return true;
}

void APlayerShip::OnFire(const FInputActionValue& /*Value*/)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const float Now = World->GetTimeSeconds();
	if (Now - LastFireTime < FireCooldown)
	{
		return;
	}
	LastFireTime = Now;

	const FVector Dir = UOrbitMath::RadialDirection(CurrentAngleDeg);
	const FVector SpawnLoc = GetActorLocation() + Dir * MuzzleOffset;

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UClass* ClassToSpawn = ProjectileClass.Get();
	if (!ClassToSpawn)
	{
		ClassToSpawn = AProjectile::StaticClass();
	}

	if (AProjectile* Proj = World->SpawnActor<AProjectile>(ClassToSpawn, SpawnLoc, Dir.Rotation(), Params))
	{
		Proj->InitProjectile(Dir, EProjectileTeam::Player);
	}
}



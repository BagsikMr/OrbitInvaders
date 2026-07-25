

#include "AsteroidSpawner.h"
#include "Asteroid.h"
#include "Engine/World.h"


AAsteroidSpawner::AAsteroidSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAsteroidSpawner::BeginPlay()
{
	Super::BeginPlay();
	BaseMinInterval = MinSpawnInterval;
	BaseMaxInterval = MaxSpawnInterval;
	BaseMinSpeed = MinSpeed;
	BaseMaxSpeed = MaxSpeed;
	TimeToNext = RollInterval();
}

void AAsteroidSpawner::ApplyWave(int32 WaveNumber)
{
	const int32 Extra = FMath::Max(0, WaveNumber - 1);
	const float RateFactor = 1.f + 0.10f * Extra;   // spawn more often
	const float SpeedFactor = 1.f + 0.08f * Extra;  // fly faster

	MinSpawnInterval = FMath::Max(0.4f, BaseMinInterval / RateFactor);
	MaxSpawnInterval = FMath::Max(0.8f, BaseMaxInterval / RateFactor);
	MinSpeed = BaseMinSpeed * SpeedFactor;
	MaxSpeed = BaseMaxSpeed * SpeedFactor;
}

void AAsteroidSpawner::ApplySettings(const FAsteroidSettings& InSettings)
{
	MinSpawnInterval = InSettings.MinSpawnInterval;
	MaxSpawnInterval = InSettings.MaxSpawnInterval;
	MinSpeed = InSettings.MinSpeed;
	MaxSpeed = InSettings.MaxSpeed;
	BaseMinInterval = MinSpawnInterval;
	BaseMaxInterval = MaxSpawnInterval;
	BaseMinSpeed = MinSpeed;
	BaseMaxSpeed = MaxSpeed;
	TimeToNext = RollInterval();
}


float AAsteroidSpawner::RollInterval() const
{
	return FMath::FRandRange(MinSpawnInterval, MaxSpawnInterval);
}

void AAsteroidSpawner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TimeToNext -= DeltaSeconds;
	if (TimeToNext <= 0.f)
	{
		SpawnOne();
		TimeToNext = RollInterval();
	}
}

void AAsteroidSpawner::SpawnOne()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UClass* ClassToSpawn = AsteroidClass ? AsteroidClass.Get() : AAsteroid::StaticClass();
	if (!ClassToSpawn)
	{
		return;
	}

	// Pick a random edge and a crossing direction.
	FVector SpawnLoc = FVector::ZeroVector;
	FVector Dir = FVector::ZeroVector;
	const int32 Edge = FMath::RandRange(0, 3);
	switch (Edge)
	{
	case 0: // left -> right
		SpawnLoc = FVector(-HalfBounds.X, 0.f, FMath::FRandRange(-HalfBounds.Y, HalfBounds.Y));
		Dir = FVector(1.f, 0.f, FMath::FRandRange(-0.5f, 0.5f));
		break;
	case 1: // right -> left
		SpawnLoc = FVector(HalfBounds.X, 0.f, FMath::FRandRange(-HalfBounds.Y, HalfBounds.Y));
		Dir = FVector(-1.f, 0.f, FMath::FRandRange(-0.5f, 0.5f));
		break;
	case 2: // top -> bottom
		SpawnLoc = FVector(FMath::FRandRange(-HalfBounds.X, HalfBounds.X), 0.f, HalfBounds.Y);
		Dir = FVector(FMath::FRandRange(-0.5f, 0.5f), 0.f, -1.f);
		break;
	default: // bottom -> top
		SpawnLoc = FVector(FMath::FRandRange(-HalfBounds.X, HalfBounds.X), 0.f, -HalfBounds.Y);
		Dir = FVector(FMath::FRandRange(-0.5f, 0.5f), 0.f, 1.f);
		break;
	}
	Dir = Dir.GetSafeNormal();

	// Weighted random size (mostly big/medium so splitting matters).
	const float Roll = FMath::FRand();
	EAsteroidSize Size = EAsteroidSize::Big;
	if (Roll > 0.5f && Roll <= 0.85f)
	{
		Size = EAsteroidSize::Medium;
	}
	else if (Roll > 0.85f)
	{
		Size = EAsteroidSize::Small;
	}

	const float Speed = FMath::FRandRange(MinSpeed, MaxSpeed);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (AAsteroid* Asteroid = World->SpawnActor<AAsteroid>(ClassToSpawn, SpawnLoc, FRotator::ZeroRotator, Params))
	{
		Asteroid->SetVisual(AsteroidVisual);
		Asteroid->InitAsteroid(Size, Dir * Speed, HalfBounds);
	}
}



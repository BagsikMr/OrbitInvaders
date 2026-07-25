
#include "InvaderFormation.h"
#include "EnemyInvader.h"
#include "OrbitMath.h"
#include "OrbitInvadersGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AInvaderFormation::AInvaderFormation()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AInvaderFormation::BeginPlay()
{
	Super::BeginPlay();
}

void AInvaderFormation::ApplySettings(const FInvaderSettings& InSettings)
{
	Rows = InSettings.Rows;
	Columns = InSettings.Columns;
	AngularSpeed = InSettings.AngularSpeed;
	DescendStep = InSettings.DescendStep;
	EnemyFireInterval = InSettings.EnemyFireInterval;
	ColumnSpacingDeg = InSettings.ColumnSpacingDeg;
}

void AInvaderFormation::StartWave(int32 WaveNumber, bool bEndless)
{
	if (bEndless)
	{
		const int32 Extra = FMath::Max(0, WaveNumber - 1);
		Columns = FMath::Min(Columns + Extra, 16);
		Rows = FMath::Min(Rows + Extra / 3, 6);
		AngularSpeed *= (1.f + 0.12f * Extra);
		DescendStep *= (1.f + 0.05f * Extra);
		EnemyFireInterval = FMath::Max(0.25f, EnemyFireInterval / (1.f + 0.10f * Extra));
	}

	SpawnFormation();
	UpdatePositions();
}

void AInvaderFormation::SpawnFormation()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UClass* ClassToSpawn = InvaderClass.Get();

	if (!ClassToSpawn)
	{
		ClassToSpawn = AEnemyInvader::StaticClass();
	}

	UE_LOG(LogTemp, Warning, TEXT("InvaderFormation: InvaderClass=%s -> using=%s (Rows=%d Cols=%d)"),
		*GetNameSafe(InvaderClass.Get()), *GetNameSafe(ClassToSpawn), Rows, Columns);

	if (!ClassToSpawn)
	{
		UE_LOG(LogTemp, Error, TEXT("InvaderFormation: AEnemyInvader::StaticClass() is NULL - check the build!"));
		return;
	}

	RowRadius.SetNum(Rows);
	for (int32 r = 0; r < Rows; ++r)
	{
		RowRadius[r] = OuterRadius - r * RowSpacing; // row 0 = outermost
	}

	const float RingStep = (Columns > 0) ? (360.f / Columns) : 0.f;
	const float HalfSpan = 0.5f * (Columns - 1) * ColumnSpacingDeg;

	for (int32 r = 0; r < Rows; ++r)
	{
		for (int32 c = 0; c < Columns; ++c)
		{
			const float BaseAngle = bSurroundEarth
				? (c * RingStep + r * RingRowStaggerDeg)
				: (CenterAngleDeg - HalfSpan + c * ColumnSpacingDeg);
			const FVector Loc = UOrbitMath::PolarToWorld(RowRadius[r], BaseAngle, 0.f);

			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AEnemyInvader* Invader = World->SpawnActor<AEnemyInvader>(ClassToSpawn, Loc, FRotator::ZeroRotator, Params);
			if (!Invader)
			{
				continue;
			}
			Invader->SetFormation(this);
			Invader->SetVisual(InvaderVisual);

			FInvaderSlot Slot;
			Slot.Invader = Invader;
			Slot.Row = r;
			Slot.BaseAngleDeg = BaseAngle;
			Slots.Add(Slot);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("InvaderFormation: spawned %d invaders"), Slots.Num());
}

void AInvaderFormation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bGameOver)
	{
		return;
	}

	SwingOffset += SwingDir * AngularSpeed * DeltaTime;
	if (FMath::Abs(SwingOffset) >= SwingMarginDeg)
	{
		SwingOffset = FMath::Sign(SwingOffset) * SwingMarginDeg;
		SwingDir = -SwingDir;
		StepInward();
	}

	UpdatePositions();
	TryFire(DeltaTime);
	CheckLose();
}

void AInvaderFormation::UpdatePositions()
{
	for (const FInvaderSlot& Slot : Slots)
	{
		AEnemyInvader* Inv = Slot.Invader;
		if (!IsValid(Inv))
		{
			continue;
		}
		const float Angle = Slot.BaseAngleDeg + SwingOffset;
		const FVector Loc = UOrbitMath::PolarToWorld(RowRadius[Slot.Row], Angle, 0.f);
		const FRotator Rot = UOrbitMath::FacingOutwardRotation(Angle);

		Inv->SetActorLocationAndRotation(Loc, Rot);
	}
}

void AInvaderFormation::StepInward()
{
	for (float& R : RowRadius)
	{
		R -= DescendStep;
	}
}

void AInvaderFormation::TryFire(float DeltaTime)
{
	FireTimer += DeltaTime;
	if (FireTimer < EnemyFireInterval)
	{
		return;
	}
	FireTimer = 0.f;

	TArray<AEnemyInvader*> Alive;
	for (const FInvaderSlot& Slot : Slots)
	{
		if (IsValid(Slot.Invader))
		{
			Alive.Add(Slot.Invader);
		}
	}
	if (Alive.Num() > 0)
	{
		const int32 Index = FMath::RandRange(0, Alive.Num() - 1);
		Alive[Index]->FireAtEarth();
	}
}

void AInvaderFormation::CheckLose()
{
	for (const FInvaderSlot& Slot : Slots)
	{
		if (IsValid(Slot.Invader) && RowRadius[Slot.Row] <= LoseRadius)
		{
			bGameOver = true;
			if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
			{
				GM->EndGame(false);
			}
			return;
		}
	}
}

int32 AInvaderFormation::CountAlive() const
{
	int32 Count = 0;
	for (const FInvaderSlot& Slot : Slots)
	{
		if (IsValid(Slot.Invader))
		{
			++Count;
		}
	}
	return Count;
}

void AInvaderFormation::NotifyInvaderDestroyed(AEnemyInvader* Invader)
{
	for (FInvaderSlot& Slot : Slots)
	{
		if (Slot.Invader == Invader)
		{
			Slot.Invader = nullptr;
			break;
		}
	}

	if (AOrbitInvadersGameMode* GM = Cast<AOrbitInvadersGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->AddScore(10);
		if (CountAlive() == 0 && !bGameOver)
		{
			bGameOver = true;
			GM->OnWaveCleared();
		}
	}
}



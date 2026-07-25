
#pragma once
#include "CoreMinimal.h"
#include "OrbitGameSettings.generated.h"
class UStaticMesh;
class UMaterialInterface;
class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct FActorVisual
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TObjectPtr<UStaticMesh> Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	TObjectPtr<UMaterialInterface> Material = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual", meta = (ClampMin = "0"))
	float UniformScale = 0.f;
};

USTRUCT(BlueprintType)
struct FPlayerSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (ClampMin = "10", ClampMax = "720"))
	float AngularSpeed = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (ClampMin = "0.05", ClampMax = "2"))
	float FireCooldown = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (ClampMin = "1", ClampMax = "10"))
	int32 MaxLives = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (ClampMin = "0", ClampMax = "3"))
	float InvulnAfterHit = 0.8f;
};

USTRUCT(BlueprintType)
struct FInvaderSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invaders", meta = (ClampMin = "1", ClampMax = "6"))
	int32 Rows = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invaders", meta = (ClampMin = "1", ClampMax = "16"))
	int32 Columns = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invaders", meta = (ClampMin = "1", ClampMax = "120"))
	float AngularSpeed = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invaders", meta = (ClampMin = "0", ClampMax = "200"))
	float DescendStep = 55.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invaders", meta = (ClampMin = "0.2", ClampMax = "5"))
	float EnemyFireInterval = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invaders", meta = (ClampMin = "4", ClampMax = "40"))
	float ColumnSpacingDeg = 14.f;
};

USTRUCT(BlueprintType)
struct FAsteroidSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroids", meta = (ClampMin = "0.3", ClampMax = "10"))
	float MinSpawnInterval = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroids", meta = (ClampMin = "0.5", ClampMax = "15"))
	float MaxSpawnInterval = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroids", meta = (ClampMin = "50", ClampMax = "1500"))
	float MinSpeed = 220.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroids", meta = (ClampMin = "80", ClampMax = "2000"))
	float MaxSpeed = 450.f;
};

USTRUCT(BlueprintType)
struct FSpecialSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special", meta = (ClampMin = "2", ClampMax = "60"))
	float SpawnInterval = 12.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special", meta = (ClampMin = "1", ClampMax = "20"))
	int32 MaxHP = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special", meta = (ClampMin = "10", ClampMax = "360"))
	float AngularSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special", meta = (ClampMin = "10", ClampMax = "400"))
	float RadialSpeed = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special", meta = (ClampMin = "1", ClampMax = "10"))
	int32 EarthDamage = 3;
};

USTRUCT(BlueprintType)
struct FEarthSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Earth", meta = (ClampMin = "1", ClampMax = "100"))
	int32 MaxHP = 10;
};

USTRUCT(BlueprintType)
struct FBunkerSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bunkers", meta = (ClampMin = "0", ClampMax = "16"))
	int32 BunkerCount = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bunkers", meta = (ClampMin = "1", ClampMax = "30"))
	int32 MaxHP = 6;
};

USTRUCT(BlueprintType)
struct FOrbitGameSettings
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FPlayerSettings Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FInvaderSettings Invaders;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FAsteroidSettings Asteroids;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FSpecialSettings Special;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FEarthSettings Earth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FBunkerSettings Bunkers;
};

namespace OrbitVisual
{
	ORBITINVADERS_API void Apply(UStaticMeshComponent* MeshComp, const FActorVisual& Visual, bool bApplyScale = true);
}
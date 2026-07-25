

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OrbitMath.generated.h"

/**
 * 
 */
UCLASS()
class ORBITINVADERS_API UOrbitMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "OrbitMath")
	static FVector PolarToWorld(float Radius, float AngleDeg, float DepthY = 0.f);

	UFUNCTION(BlueprintPure, Category = "OrbitMath")
	static void WorldToPolar(const FVector& World, float& OutRadius, float& OutAngleDeg);

	UFUNCTION(BlueprintPure, Category = "OrbitMath")
	static float WrapAngle(float AngleDeg);

	UFUNCTION(BlueprintPure, Category = "OrbitMath")
	static FVector RadialDirection(float AngleDeg);

	UFUNCTION(BlueprintPure, Category = "OrbitMath")
	static FRotator FacingOutwardRotation(float AngleDeg);
};

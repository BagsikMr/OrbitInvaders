


#include "OrbitMath.h"


FVector UOrbitMath::PolarToWorld(float Radius, float AngleDeg, float DepthY)
{
	const float Rad = FMath::DegreesToRadians(AngleDeg);
	return FVector(Radius * FMath::Cos(Rad), DepthY, Radius * FMath::Sin(Rad));
}

void UOrbitMath::WorldToPolar(const FVector& World, float& OutRadius, float& OutAngleDeg)
{
	OutRadius = FMath::Sqrt(World.X * World.X + World.Z * World.Z);
	OutAngleDeg = WrapAngle(FMath::RadiansToDegrees(FMath::Atan2(World.Z, World.X)));
}

float UOrbitMath::WrapAngle(float AngleDeg)
{
	float A = FMath::Fmod(AngleDeg, 360.f);
	if (A < 0.f)
	{
		A += 360.f;
	}
	return A;
}

FVector UOrbitMath::RadialDirection(float AngleDeg)
{
	const float Rad = FMath::DegreesToRadians(AngleDeg);
	return FVector(FMath::Cos(Rad), 0.f, FMath::Sin(Rad));
}

FRotator UOrbitMath::FacingOutwardRotation(float AngleDeg)
{
	const FVector Outward = RadialDirection(AngleDeg);
	const FVector Forward = FVector(0.f, -1.f, 0.f); 
	return FRotationMatrix::MakeFromXZ(Forward, Outward).Rotator();
}


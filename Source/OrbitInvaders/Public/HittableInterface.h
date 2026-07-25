

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Projectile.h"
#include "HittableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHittableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 67
 */
class ORBITINVADERS_API IHittableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual bool ReceiveProjectileHit(EProjectileTeam ByTeam) = 0;
};

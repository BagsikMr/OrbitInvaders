
#include "OrbitGameSettings.h"
#include "Components/StaticMeshComponent.h"


void OrbitVisual::Apply(UStaticMeshComponent* MeshComp, const FActorVisual& Visual, bool bApplyScale)
{
	if (!MeshComp)
	{
		return;
	}

	if (Visual.Mesh)
	{
		MeshComp->SetStaticMesh(Visual.Mesh);
	}

	if (Visual.Material)
	{
		MeshComp->SetMaterial(0, Visual.Material);
	}

	if (bApplyScale && Visual.UniformScale > 0.f)
	{
		MeshComp->SetRelativeScale3D(FVector(Visual.UniformScale));
	}
}
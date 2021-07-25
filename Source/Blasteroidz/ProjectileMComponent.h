// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Asteroid.h"
#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileMComponent.generated.h"

/**
 * 
 */
UCLASS()
class BLASTEROIDZ_API UProjectileMComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

	AAsteroid* Asteroid;

	//FVector ComputeVelocity(FVector InitialVelocity, float DeltaTime);

	

public:
	UPROPERTY(Category = "Asteroid Movement", BlueprintReadWrite, EditAnywhere)
		//FROM 0-1
		float AsteroidDrag = 0.95f;

	void Drag(FVector InVelocity, FVector& NewVelocity, float DeltaTime, float Drag);

	

};

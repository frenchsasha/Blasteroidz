// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PlayerShipCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class BLASTEROIDZ_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	APlayerShipCharacter* PlayerShipCharacter;

	virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;

	void Drag(FVector Velocity, FVector& NewVelocity, float DeltaTime, float PlayerDrag);


public:
	UPROPERTY(Category = "Player Movement", BlueprintReadWrite, EditAnywhere)
		//FROM 0-1
		float PlayerDrag = 0.95f;

	UPROPERTY(Category = "Player Movement", BlueprintReadWrite, EditAnywhere)
		float PlayerAcceleration = 100.f;
};

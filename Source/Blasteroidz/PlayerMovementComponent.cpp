// Fill out your copyright notice in the Description page of Project Settings.

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <cmath>
#include "PlayerMovementComponent.h"

void UPlayerMovementComponent::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
{

	PlayerShipCharacter = Cast<APlayerShipCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	Drag(Velocity, Velocity, DeltaTime, PlayerDrag);
	Velocity += PlayerShipCharacter->GetActorForwardVector()*PlayerShipCharacter->forwardIn*PlayerAcceleration*DeltaTime;


	//VISUALIZE
	float sSpeed = (Velocity.Size());
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::White, FString::Printf(TEXT("%f"),sSpeed));
	}


}

void UPlayerMovementComponent::Drag(FVector InVelocity, FVector &NewVelocity, float DeltaTime, float Drag)
{
	NewVelocity = InVelocity;
	float speed, drop, ScaledSpeed;

	speed = Velocity.Size();
	drop = speed * Drag * DeltaTime;
	ScaledSpeed = speed-drop;


	//the only reason this is here is for when game starts up

	if (ScaledSpeed < 0)
		ScaledSpeed = 0;
	if (ScaledSpeed > 0)
		ScaledSpeed /= speed;

	NewVelocity *= ScaledSpeed;


}

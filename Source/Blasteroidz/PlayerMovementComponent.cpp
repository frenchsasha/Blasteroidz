
// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <cmath>


void UPlayerMovementComponent::CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration)
{

	PlayerShipCharacter = Cast<APlayerShipCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	Drag(Velocity, Velocity, DeltaTime, PlayerDrag);
	
	Velocity += PlayerShipCharacter->GetActorForwardVector()*PlayerShipCharacter->forwardIn*PlayerAcceleration*DeltaTime* Boost(BoostDuration, DeltaTime);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::White, FString::Printf(TEXT("accel: %f"), PlayerAcceleration* Boost(BoostDuration, DeltaTime)));
	}
	

	//VISUALIZE
	float sSpeed = (Velocity.Size());
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::White, FString::Printf(TEXT("Player Speed: %f"), sSpeed));
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

float UPlayerMovementComponent::Boost(float Duration, float DeltaTime)
{
	
	

	bool BoostIn = Cast<APlayerShipCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->boostIn;


	if (BoostIn) {
		isBoost = true;
	}

	if (isBoost == true) {
		timer = FMath::Clamp(timer + DeltaTime, 0.f, Duration);
		
		float Scale = Accel->GetFloatValue(timer / Duration);
	
		if (timer >= Duration) {
			isBoost = false;
			timer = 0.f;
	
		}

		

		return (BoostMult*(1.0)); //+Scale

	
		
	}
	else {

		

		return 1.f;
	}

	
	

	

}

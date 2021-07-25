// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileMComponent.h"

//FVector UProjectileMComponent::ComputeVelocity(FVector InitialVelocity, float DeltaTime) 
//{
//	// v = v0 + a*t
//	const FVector Acceleration = ComputeAcceleration(InitialVelocity, DeltaTime);
//	FVector NewVelocity = InitialVelocity + (Acceleration * DeltaTime);
//	Drag(NewVelocity, NewVelocity, DeltaTime, AsteroidDrag);
//
//	return LimitVelocity(NewVelocity);
//}




void UProjectileMComponent::Drag(FVector InVelocity, FVector& NewVelocity, float DeltaTime, float Drag)
{
	NewVelocity = InVelocity;
	float speed, drop, ScaledSpeed;

	speed = InVelocity.Size();
	drop = speed * Drag * DeltaTime;
	ScaledSpeed = speed - drop;


	//the only reason this is here is for when game starts up

	if (ScaledSpeed < 0)
		ScaledSpeed = 0;
	if (ScaledSpeed > 0)
		ScaledSpeed /= speed;

	NewVelocity *= ScaledSpeed;


}

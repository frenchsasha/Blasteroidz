// Fill out your copyright notice in the Description page of Project Settings.

#include "Asteroid.h"
#include "GeneralMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GravityComponent.h"

// Sets default values
AAsteroid::AAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("STATIC MESH"));
	//Collision = CreateDefaultSubobject<USphereComponent>(TEXT("CUSTOM COLLISION"));
	GeneralMovementComponent = CreateDefaultSubobject<UGeneralMovementComponent>(TEXT("CUSTOM MOVEMENT"));
	GravityComponent = CreateDefaultSubobject<UGravityComponent>(TEXT("Gravity Component"));
	
	
	StaticMesh->CanEditSimulatePhysics();
	

}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//VISUALIZE

	 
//	float sSpeed = (AAsteroid::GetVelocity().Size());
//	if (GEngine) {
//		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::White, FString::Printf(TEXT("Asteroid Speed: %f"), sSpeed));
//	}
}


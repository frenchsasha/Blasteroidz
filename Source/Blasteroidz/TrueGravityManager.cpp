// Fill out your copyright notice in the Description page of Project Settings.


#include "TrueGravityManager.h"
#include "EngineUtils.h"
#include "Asteroid.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "GravityManager.h"
#include "ProjectileMComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GeneralMovementComponent.h"
#include "PlayerShipCharacter.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

#include "PhysicsEngine/AggregateGeom.h"
#include "PhysicsEngine/SphereElem.h"
#include "PhysicsEngine/BodySetup.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


//#include cmath
// Sets default values
ATrueGravityManager::ATrueGravityManager()
{
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Get Player Data
	if (PlayerShipCharacter) {
		UCapsuleComponent* PlayerCollision = PlayerShipCharacter->FindComponentByClass<UCapsuleComponent>();
		float PlayerColRadius = PlayerCollision->GetScaledCapsuleRadius();

		//Get Player Location, Mass and Radius

		float PlayerMass = PlayerShipCharacter->Gravity->Mass;
		float PlayerGravRadius = PlayerShipCharacter->Gravity->Radius;
	}
	

}

// Called when the game starts or when spawned
void ATrueGravityManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerShipCharacter = Cast<APlayerShipCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));



}

// Called every frame
void ATrueGravityManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float PlayerColRadius = 0.f;
	FVector PlayerLocation = FVector::ZeroVector;
	float PlayerMass = 0.f;
	float PlayerGravRadius = 0.f;

	
	PlayerLocation = PlayerShipCharacter->GetActorLocation();
	//Per Asteroid
	for (TActorIterator<AAsteroid> Current(GetWorld()); Current; ++Current) {
		float CurrentColRadius = 0.f;


		float CurrentToPlayerDistance = 0.f;
		float CurrentToPlayerAcceleration = 0.f;

		//Get Current Location, Mass and Radius
		FVector CurrentLocation = Current->GetActorLocation();
		float CurrentMass = Current->Gravity->Mass;
		float CurrentGravRadius = Current->Gravity->Radius;
		float CurrentAsteroidDrag = Current->Gravity->AsteroidDrag;
		FVector CurrentScale = Current->GetActorTransform().GetScale3D();
		FVector CurrentVelocity = Current->GetVelocity();


		//Get Collision Radius from Static Mesh
		UStaticMeshComponent* CurrentMeshComponent = Current->FindComponentByClass<UStaticMeshComponent>();
		UStaticMesh* CurrentStaticMesh = CurrentMeshComponent->GetStaticMesh();
		struct FKAggregateGeom CurrentGeo = CurrentMeshComponent->GetBodySetup()->AggGeom;
		TArray<FKSphereElem> CurrentMeshSphere = CurrentGeo.SphereElems;
		for (FKSphereElem CurrentMeshSphereElem : CurrentMeshSphere) {
			CurrentColRadius = CurrentMeshSphereElem.Radius;
		}

		CurrentColRadius *= CurrentScale.X;

		//Get Vector To Player
		FVector ToPlayer = PlayerLocation - CurrentLocation;
		ToPlayer.Normalize();

			//VISUALIZE Asteroid to Player
			DrawDebugLine(GetWorld(), CurrentLocation, PlayerLocation, FColor::Purple, false, -1.f, 10.f);

		//Gravity Calculations From Player To Asteroid
		CurrentToPlayerDistance = (CurrentLocation - PlayerLocation).Size();
		CurrentToPlayerAcceleration = (GravitationalConstant * PlayerGravRadius * PlayerMass) / (CurrentToPlayerDistance * CurrentToPlayerDistance);

		//Only Add Force if Not Intersecting
		if ((PlayerLocation - CurrentLocation).Size() > PlayerColRadius + CurrentColRadius) {
			CurrentMeshComponent->AddForce(-CurrentVelocity * AsteroidDrag*CurrentAsteroidDrag, NAME_None, true);
			CurrentMeshComponent->AddForce(ToPlayer * CurrentToPlayerAcceleration, NAME_None, true);
		}


			//VISUALIZE
			float dist = sqrt((GravitationalConstant * Current->Gravity->Radius * Current->Gravity->Mass) / MinAccel);
			FMatrix AsteroidMatrix = FMatrix(FVector::UpVector, FVector::ForwardVector, FVector::RightVector, CurrentLocation);
			DrawDebugCircle(GetWorld(), AsteroidMatrix, CurrentColRadius, 64, FColor::Blue, false, -1.f, 1, 5.f);
			//DrawDebugCircle(GetWorld(), AsteroidMatrix, dist, 64, FColor::Red, false, -1.f, 1, 5.f);
			

			//VISUALIZE
		//	float dist = sqrt((GravitationalConstant * PlayerShipCharacter->Gravity->Radius * PlayerShipCharacter->Gravity->Mass) / MinAccel);
		//	FMatrix PlayerMatrix = FMatrix(FVector::UpVector, FVector::ForwardVector, FVector::RightVector, PlayerLocation);
		//	DrawDebugCircle(GetWorld(), PlayerMatrix, PlayerColRadius, 64, FColor::Green, false, -1.f, 1, 5.f);
		//	DrawDebugCircle(GetWorld(), PlayerMatrix, dist, 64, FColor::Red, false, -1.f, 1, 5.f);

		

		//Per Target Asteroid
		for (TActorIterator<AAsteroid> Target(GetWorld()); Target; ++Target) {
			//Do not affect yourself
			if (*Current != *Target) {

				float TargetColRadius = 0.f;

				float TargetToCurrentDistance = 0.f;
				float TargetToCurrentAcceleration = 0.f;

				//Get Target Location, Mass and Radius
				FVector TargetLocation = Target->GetActorLocation();
				float TargetMass = Target->Gravity->Mass;
				float TargetGravRadius = Target->Gravity->Radius;
				float TargetAsteroidDrag = Target->Gravity->AsteroidDrag;
				FVector TargetScale = Target->GetActorTransform().GetScale3D();
				FVector TargetVelocity = Target->GetVelocity();


				//Get Collision Radius from Static Mesh
				UStaticMeshComponent* TargetMeshComponent = Target->FindComponentByClass<UStaticMeshComponent>();
				UStaticMesh* TargetStaticMesh = TargetMeshComponent->GetStaticMesh();
				struct FKAggregateGeom TargetGeo = TargetMeshComponent->GetBodySetup()->AggGeom;
				TArray<FKSphereElem> TargetMeshSphere = TargetGeo.SphereElems;
				for (FKSphereElem TargetMeshSphereElem : TargetMeshSphere) {
					TargetColRadius = TargetMeshSphereElem.Radius;
				}

				TargetColRadius *= TargetScale.X;

				//Get Vector to Target Asteroid
				FVector ToCurrent = TargetLocation-CurrentLocation;
				ToCurrent.Normalize();

				//VISUALIZE
				DrawDebugLine(GetWorld(), CurrentLocation, TargetLocation, FColor::Purple, false, -1.f, 10.f);

				//Calculate Gravity Force from Current To Target
				TargetToCurrentDistance = (CurrentLocation-TargetLocation).Size();
				TargetToCurrentAcceleration = (GravitationalConstant * TargetGravRadius * TargetMass) / (TargetToCurrentDistance * TargetToCurrentDistance);

				//Only Add Force if Not Intersecting
				if ((CurrentLocation-TargetLocation).Size() > TargetGravRadius + CurrentGravRadius) {
					CurrentMeshComponent->AddForce(-CurrentVelocity * AsteroidDrag*CurrentAsteroidDrag, NAME_None, true);
					CurrentMeshComponent->AddForce(ToCurrent * TargetToCurrentAcceleration, NAME_None, true);
					//Why did this suddenly work???
					
						//VISUALIZE
						float MinDist = sqrt((GravitationalConstant * Current->Gravity->Radius * Current->Gravity->Mass) / MinAccel);
						//FMatrix AsteroidMatrix = FMatrix(FVector::UpVector, FVector::ForwardVector, FVector::RightVector, CurrentLocation);
						DrawDebugCircle(GetWorld(), AsteroidMatrix, CurrentGravRadius, 64, FColor::Green, false, -1.f, 1, 5.f);
						DrawDebugCircle(GetWorld(), AsteroidMatrix, MinDist, 64, FColor::Red, false, -1.f, 1, 5.f);
				}

			}

		}

	}
	
}


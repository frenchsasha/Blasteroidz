// Fill out your copyright notice in the Description page of Project Settings.


#include "TrueGravityManager.h"
#include "EngineUtils.h"
#include "Asteroid.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "GravityComponent.h"
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
#include "Math/UnrealMathUtility.h"

//#include cmath
// Sets default values
ATrueGravityManager::ATrueGravityManager()
{


	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	




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


	//Per Asteroid in the world 
	for (TActorIterator<AAsteroid> Current(GetWorld()); Current; ++Current) {
		float CurrentColRadius = 0.f;

		//Get Current Location, Mass and Radius
		FVector CurrentLocation = Current->GetActorLocation();
		float CurrentMass = Current->GravityComponent->Mass;
		float CurrentGravRadius = Current->GravityComponent->Radius;
		float CurrentAsteroidDrag = Current->GravityComponent->AsteroidDrag;
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



		

		//Per Target Asteroid
		for (TActorIterator<AAsteroid> Target(GetWorld()); Target; ++Target) {
			//Do not affect yourself

			float TargetColRadius = 0.f;
			float TargetToCurrentDistance = 0.f;
			float TargetToCurrentAcceleration = 0.f;
			float CurrentToTargetAcceleration = 0.f;

			//Get Target Location, Mass and Radius
			FVector TargetLocation = Target->GetActorLocation();
			float TargetMass = Target->GravityComponent->Mass;
			float TargetGravRadius = Target->GravityComponent->Radius;
			float TargetAsteroidDrag = Target->GravityComponent->AsteroidDrag;
			FVector TargetScale = Target->GetActorTransform().GetScale3D();
			FVector TargetVelocity = Target->GetVelocity();


			//Get Mesh (needed to apply force to the mesh)
			UStaticMeshComponent* TargetMeshComponent = Target->FindComponentByClass<UStaticMeshComponent>();
			UStaticMesh* TargetStaticMesh = TargetMeshComponent->GetStaticMesh();
			struct FKAggregateGeom TargetGeo = TargetMeshComponent->GetBodySetup()->AggGeom;
			TArray<FKSphereElem> TargetMeshSphere = TargetGeo.SphereElems;
			for (FKSphereElem TargetMeshSphereElem : TargetMeshSphere) {
				TargetColRadius = TargetMeshSphereElem.Radius;
			}

			TargetColRadius *= TargetScale.X;



			if (*Current != *Target  ) {

				float TargetCurrentRadius = TargetColRadius + CurrentColRadius;
				//(CurrentLocation - TargetLocation).Size()
				
				//VISUALIZE
				float dist = 2.f*(GravitationalConstant * Current->GravityComponent->Mass) / (MinAccel*MinAccel);
				FMatrix AsteroidMatrix = FMatrix(FVector::UpVector, FVector::ForwardVector, FVector::RightVector, CurrentLocation);
				DrawDebugCircle(GetWorld(), AsteroidMatrix, CurrentColRadius, 64, FColor::Blue, false, -1.f, 1, 5.f);
				DrawDebugCircle(GetWorld(), AsteroidMatrix, dist, 64, FColor::Red, false, -1.f, 1, 5.f);
				//DrawDebugString(GetWorld(), CurrentLocation + (CurrentLocation + TargetLocation) / 2.f, scale.ToString());
				
				//Get Vector to Current Asteroid
				FVector ToCurrent = CurrentLocation - TargetLocation;
				ToCurrent.Normalize();


				//Calculate Gravity Force from Target to Current
				TargetToCurrentDistance = (CurrentLocation - TargetLocation).Size();

				

				TargetToCurrentAcceleration = (GravitationalConstant * CurrentMass) / (TargetToCurrentDistance * TargetToCurrentDistance);
				float scale = FMath::Clamp((TargetToCurrentDistance - (TargetColRadius + CurrentColRadius)) / (TargetColRadius + CurrentColRadius),0.f, 1.f);
				FVector2D in = (TargetCurrentRadius,300.f);
				FVector2D out = (1.f, 1.f);
				//float scale = out.X + (TargetToCurrentDistance/(in.Y-in.X))*(out.Y-out.X);

				//float scale = FMath::Clamp(((TargetToCurrentDistance) / (TargetCurrentRadius)),0.f,1.f) ;


				//if (GEngine) {
				//	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::White, FString::Printf(TEXT("Scale: %f"), scale));
				//}
				TargetMeshComponent->AddForce(ToCurrent * TargetToCurrentAcceleration*scale, NAME_None, true);


			}

		}

	}

}
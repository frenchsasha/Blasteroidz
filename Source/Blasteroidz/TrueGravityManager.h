// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerShipCharacter.h"
#include "TrueGravityManager.generated.h"

UCLASS()
class BLASTEROIDZ_API ATrueGravityManager : public AActor
{
	GENERATED_BODY()
	
public:	


	// Sets default values for this actor's properties
	ATrueGravityManager();

	APlayerShipCharacter* PlayerShipCharacter;

	UPROPERTY(Category = "Gravity", BlueprintReadWrite, EditAnywhere)
	float GravitationalConstant = 0.3f;

	UPROPERTY(Category = "Asteroid Movement", BlueprintReadWrite, EditAnywhere)
		//FROM 0-1
		float AsteroidDrag = 0.5f;

	UPROPERTY(Category = "Visualize", BlueprintReadWrite, EditAnywhere)
		//FROM 0-1
		float MinAccel = 0.001f;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

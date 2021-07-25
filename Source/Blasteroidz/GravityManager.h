// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTEROIDZ_API UGravityManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGravityManager();

	UPROPERTY(Category = "Asteroid", BlueprintReadWrite, EditAnywhere)
		float Radius = 16.f;
	UPROPERTY(Category = "Asteroid", BlueprintReadWrite, EditAnywhere)
		float Mass = 800.f;
	UPROPERTY(Category = "Asteroid", BlueprintReadWrite, EditAnywhere)
		float AsteroidDrag = 0.2f;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

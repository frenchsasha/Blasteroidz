// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerShipCharacter.generated.h"

class AMyCameraActor;

UCLASS()
class BLASTEROIDZ_API APlayerShipCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerShipCharacter();

	APlayerShipCharacter(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UGravityComponent* GravityComponent;
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//		class USpringArmComponent* SpringArm;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//		class UCameraComponent* Camera;
//
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AMyCameraActor> CameraActorToSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AMyCameraActor* MyCamera;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APlayerController* PlayerController;

	float forwardIn;

private:

	
	void forward(float value);
	void rotate(float value);

public:
	UPROPERTY(Category = "Player Movement", BlueprintReadWrite, EditAnywhere)
		float PlayerRotationRate = 2.f;
};

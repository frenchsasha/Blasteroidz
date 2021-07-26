// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerShipCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GravityComponent.h"


#include "PlayerMovementComponent.h"
#include "MyCameraActor.h"

// Sets default values
APlayerShipCharacter::APlayerShipCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GravityComponent = CreateDefaultSubobject<UGravityComponent>(TEXT("Gravity Component"));
//	Gravity = CreateDefaultSubobject<UGravityComponent>(TEXT("GRAVITY MANAGER"));
//	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
//	SpringArm->SetupAttachment(RootComponent);
//
//	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
//	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void APlayerShipCharacter::BeginPlay()
{
	
	UWorld* World = GetWorld();
	check(World);
	Super::BeginPlay();

	if (CameraActorToSpawn == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Using default!"));
		CameraActorToSpawn = AMyCameraActor::StaticClass();
	}
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	MyCamera = Cast<AMyCameraActor>(World->SpawnActor<AMyCameraActor>(CameraActorToSpawn, GetTransform(), SpawnParameters));
	check(MyCamera);
	if (APlayerController* PC = Cast<APlayerController>(GetController())) {
		PC->SetViewTarget(MyCamera);
	}
	
}

void APlayerShipCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (MyCamera)
	{
		MyCamera->Destroy();
	}
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void APlayerShipCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerShipCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Forward"), this, &APlayerShipCharacter::forward);
	PlayerInputComponent->BindAxis(FName("Rotate"), this, &APlayerShipCharacter::rotate);

}

void APlayerShipCharacter::forward(float value)
{
	forwardIn = value;
}

void APlayerShipCharacter::rotate(float value)
{
	this->AddActorLocalRotation(FRotator(0, value* PlayerRotationRate, 0));
}


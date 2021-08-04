// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerShipCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GravityComponent.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

#include "Math/Vector.h"

#include "PlayerMovementComponent.h"
#include "MyCameraActor.h"

// Sets default values
APlayerShipCharacter::APlayerShipCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerController = Cast<APlayerController>(GetController());

	GravityComponent = CreateDefaultSubobject<UGravityComponent>(TEXT("Gravity Component"));
//	Gravity = CreateDefaultSubobject<UGravityComponent>(TEXT("GRAVITY MANAGER"));
//	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
//	SpringArm->SetupAttachment(RootComponent);
//
//	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
//	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//bUseControllerDesiredRotation = true;
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

//	float MPX = 0;
//	float MPY=0;
//
//
//
//	bool GetMouse = PlayerController->GetMousePosition(MPX, MPY);
//	FVector PlayerPos = this->GetActorLocation();
//	//if (GetMouse) {
//		FVector WorldPos, WorldDir;
//		bool bScreenToWorld = PlayerController->DeprojectScreenPositionToWorld(MPX, MPY, WorldPos, WorldDir);
//		//if (bScreenToWorld) {
//			
//		//WorldPos.Z = PlayerPos.Z;
//
//			DrawDebugLine(GetWorld(),PlayerPos, WorldPos, FColor::Yellow, false, -1.f, 10.f);
//			
//			FVector LookDir = PlayerPos-WorldPos;
//			//LookDir. = 0;
//			LookDir.Normalize();
//
//			PlayerController->SetControlRotation(LookDir.Rotation());

		//FTransform PawnTransform = this->GetTransform();
		//FQuat PawnQuat = PawnTransform.GetRotation();
		//
		//FVector PawnUpVector = PawnQuat.RotateVector(FVector(0, 0, 1));
		//
		//FVector c = FVector::CrossProduct(PawnUpVector, LookDir);
		//float d = FVector::DotProduct(PawnUpVector, LookDir);
		//float s = FMath::Sqrt((1.0 + d) * 2.0);
		//float rs = 1.0 / s;
		//
		//FQuat  q(c.X * rs, c.Y * rs, c.Z * rs, s * 0.5);
		//
		//q = q * PawnQuat;
		//q.Normalize();
		//
		//PawnTransform.SetRotation(q);

			//this->GetCapsuleComponent()->SetWorldTransform(PawnTransform);

			//this->GetController()->SetControlRotation(PawnTransform.Rotator());

			//this->GetCapsuleComponent()->SetWorldTransform();
			//this->GetController()->SetControlRotation(LookDir.Rotation());
				
				//SetActorRotation(LookDir.Rotation());
			//this->SetActorRelativeRotation();
			
			//this->SetActorRotation((LookDir.ToOrientationRotator()));

		//	this->AddActorWorldTransform
			
			//this->SetActorRotation(LookDir.ToOrientationQuat());
				
				//SeRotation(LookDir.Rotation());
			//PlayerController->AddYawInput(5.0);
			// 
			//FRotator Test = (FVector(0, 0, 100)).Rotation();
	//		FRotator PCR = PlayerController->GetControlRotation();
	//
	//		//PlayerController->SetControlRotation(Test);
	//		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::White, FString::Printf(TEXT("PCR: %s"), *PCR.ToString()));
		//}
	//}


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

	PlayerController = Cast<APlayerController>(GetController());
	//FRotator Current = PlayerController->GetActorRotation();
	//if(PlayerController)
		//PlayerController->AddYawInput(value * PlayerRotationRate);
	//this->SetActorRotation(FRotator(0, value* PlayerRotationRate, 0));
}


#include "MyCameraActor.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

AMyCameraActor::AMyCameraActor()
{
	// This class doesn't tick, but all child classes (including BP-based ones!) will, so turn this on here.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
}

void AMyCameraActor::BeginPlay()
{
	Super::BeginPlay();
	// Use this to check the validity of Instigator.
	UE_LOG(LogTemp, Warning, TEXT("%s"), GetInstigator() ? *GetInstigator()->GetFName().ToString() : TEXT("null"));

	if (GetInstigator())
	{
		// We always want to tick after the actor we're tracking - it avoids visual jitter.
		PrimaryActorTick.AddPrerequisite(GetInstigator(), GetInstigator()->PrimaryActorTick);
	}
}

//TODO:
	//Modify this code so that it moves the camera up when you are accelerating
AChaseCam::AChaseCam() {

}

void AChaseCam::BeginPlay() {
	Super::BeginPlay();

	if (GetInstigator()) {
		SetActorLocation(GetInstigator()->GetActorLocation() + IdealOffset);
		GetCameraComponent()->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(IdealOffset, FVector::ZeroVector).Quaternion());
	}
}

void AChaseCam::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (GetInstigator()) {

		FVector IdealLocation = GetInstigator()->GetActorLocation() + IdealOffset;
		FVector CurrentLocation = GetActorLocation();
		float CurrentSpeed = GetVelocity().Size();
		FVector DeltaLocation = IdealLocation - CurrentLocation;
		FVector AbsDeltaLocation = FVector(FMath::Abs(DeltaLocation.X), FMath::Abs(DeltaLocation.Y), FMath::Abs(DeltaLocation.Z));
		FVector MaxMovementThisFrame = CameraSpeedPerAxis * DeltaSeconds;
		FVector DesiredMovementThisFrame;
		FVector MovementThisFrameDueToCap = FVector::ZeroVector;
		float CurrentCameraSpeedLimiter = CameraSpeedLimiter;

		//clamp deltaLocation
		//clip by radius
		FVector Direction;
		float Length;
		DeltaLocation.ToDirectionAndLength(Direction, Length);
		if (Length > CameraDistanceLimiter) {
			FVector CapVector = (Length - CameraDistanceLimiter) * Direction;
			MovementThisFrameDueToCap += CapVector;
			DeltaLocation -= CapVector;
		}


		//clip by axis
		for (int32 i = 0; i < 3; i++) {
			if (FMath::Abs(DeltaLocation.Component(i)) > CameraDistanceLimiterPerAxis.Component(i)) {
				MovementThisFrameDueToCap.Component(i) += FMath::Sign(DeltaLocation.Component(i)) * (FMath::Abs(DeltaLocation.Component(i)) - CameraDistanceLimiterPerAxis.Component(i));
				DeltaLocation.Component(i) += FMath::Clamp(DeltaLocation.Component(i), -CameraDistanceLimiterPerAxis.Component(i), CameraDistanceLimiterPerAxis.Component(i));
				AbsDeltaLocation.Component(i) = FMath::Abs(DeltaLocation.Component(i));
			}
		}


		float DeltaLocationSize = DeltaLocation.Size();

		//Radial Interpolation
		for (int32 j = 0; j < SpeedAdjusters.Num(); j++) {
			if ((SpeedAdjusters[j].AxisFlags & (1 << (int32)EChaseCamAxes::ECCA_Radius))
				&& (DeltaLocationSize == FMath::Clamp(DeltaLocationSize, SpeedAdjusters[j].DistanceRange.X, SpeedAdjusters[j].DistanceRange.Y))) {

				CurrentCameraSpeedLimiter *= FMath::GetMappedRangeValueUnclamped(SpeedAdjusters[j].DistanceRange, SpeedAdjusters[j].SpeedMultiplierRange, DeltaLocationSize);
				break;
			}
		}

		//per axis  Interpolation

		for (int32 i = (int32)EChaseCamAxes::ECCA_X; i <= (int32)EChaseCamAxes::ECCA_Z; i++) {

			for (int32 j = 0; j < SpeedAdjusters.Num(); j++) {
				if ((SpeedAdjusters[j].AxisFlags & (1 << i))
					&& (AbsDeltaLocation.Component(i) == FMath::Clamp(AbsDeltaLocation.Component(i), SpeedAdjusters[j].DistanceRange.X, SpeedAdjusters[j].DistanceRange.Y))){
					MaxMovementThisFrame.Component(i) *= FMath::GetMappedRangeValueUnclamped(SpeedAdjusters[j].DistanceRange, SpeedAdjusters[j].SpeedMultiplierRange, AbsDeltaLocation.Component(i));
}					break;
			}
		}


		for (int32 i = 0; i < 3; i++) {
			DesiredMovementThisFrame.Component(i) = FMath::Min(AbsDeltaLocation.Component(i), MaxMovementThisFrame.Component(i)) * FMath::Sign(DeltaLocation.Component(i));
		}

		
		//overeall speed clamp
		DesiredMovementThisFrame = DesiredMovementThisFrame.GetClampedToMaxSize(CurrentCameraSpeedLimiter * DeltaSeconds);

		SetActorLocation(CurrentLocation + DesiredMovementThisFrame+MovementThisFrameDueToCap);
		

		//FMath::Lerp(GetActorLocation(),)
		//SetActorLocation()

		//Debug

	#if !UE_BUILD_SHIPPING
		if (bShowDebugInfo) {
			if (UWorld* World = GetWorld()) {
				FVector DebugLineOffset = FVector(0.0f, 0.0f, -IdealOffset.Z);  //Change it if the camera height changes but you can probably just change Z offset
				FVector NewLocation = GetActorLocation();
				DrawDebugLine(GetWorld(), CurrentLocation + DebugLineOffset, NewLocation + DebugLineOffset, FColor::Red,true, 3.0f,0,10.f);
				DrawDebugBox(World, NewLocation + DebugLineOffset, CameraDistanceLimiterPerAxis, FColor::Green, false, -1.f, 1, 10.f);
				FMatrix CircleMatrix = FMatrix(FVector::UpVector, FVector::ForwardVector, FVector::RightVector, NewLocation + DebugLineOffset);
				DrawDebugCircle(World, CircleMatrix, CameraDistanceLimiter, 64, FColor::Green, false, -1.f, 1, 10.f);
			}
		}
	#endif

	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "MyCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class BLASTEROIDZ_API AMyCameraActor : public ACameraActor
{
	GENERATED_BODY()
	


public:
	AMyCameraActor();
	virtual void BeginPlay() override;
};


UENUM(Meta = (Bitflags))
enum class EChaseCamAxes {
	ECCA_X UMETA(DisplayName = "X Axis"),
	ECCA_Y UMETA(DisplayName = "Y Axis"),
	ECCA_Z UMETA(DisplayName = "Z Axis"),
	ECCA_Radius UMETA(DisplayName = "Polar (radius)")
};

USTRUCT(BlueprintType)
struct FChaseCamSpeedAdjustment {
	GENERATED_BODY()
		//used between this.X < CamDesiredDist < this.Y
		UPROPERTY(EditAnywhere)
		FVector2D DistanceRange;

	//map cam dist to dist range
	UPROPERTY(EditAnywhere)
		FVector2D SpeedMultiplierRange;

	//Applies to adjustment to axes
	UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = "EChaseCamAxes"))
		int32 AxisFlags;
};

/**
 *
 */
UCLASS()
class BLASTEROIDZ_API AChaseCam : public AMyCameraActor
{
	GENERATED_BODY()




public:
	AChaseCam();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 bShowDebugInfo : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector IdealOffset = { 0.f,0.f,500.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector CameraSpeedPerAxis = { 500.f,500.f,0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector CameraDistanceLimiterPerAxis = { 400.0f, 400.0f, 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0.0"))
		float CameraSpeedLimiter = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0.0"))
		float CameraDistanceLimiter = FLT_MAX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FChaseCamSpeedAdjustment> SpeedAdjusters;
};

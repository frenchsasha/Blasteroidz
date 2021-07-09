// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityManager.h"
#include "EngineUtils.h"

// Sets default values for this component's properties
UGravityManager::UGravityManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGravityManager::BeginPlay()
{
	Super::BeginPlay();
	int count = 0;
	for (TObjectIterator<UGravityManager> itr; itr; itr++) {
		count++;
	
	}
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::White, FString::Printf(TEXT("%d"), count));
	}

	// ...
	
}


// Called every frame
void UGravityManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


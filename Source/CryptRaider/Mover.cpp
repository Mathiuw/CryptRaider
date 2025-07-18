// Fill out your copyright notice in the Description page of Project Settings.

#include "Mover.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	originalLocation = GetOwner()->GetActorLocation();
}

// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector currentLocation = GetOwner()->GetActorLocation();

	if (canMove)
	{
		FVector desiredLocation = originalLocation + moveOffset;

		FVector newPosition = FMath::VInterpConstantTo(currentLocation, desiredLocation, DeltaTime, moveTime);

		GetOwner()->SetActorLocation(newPosition);
	}
	else
	{
		FVector newPosition = FMath::VInterpConstantTo(currentLocation, originalLocation, DeltaTime, moveTime);

		GetOwner()->SetActorLocation(newPosition);
	}

}

void UMover::SetCanMove(bool b)
{
	canMove = b;
}

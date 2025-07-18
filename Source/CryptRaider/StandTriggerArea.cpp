// Fill out your copyright notice in the Description page of Project Settings.

#include "StandTriggerArea.h"

UStandTriggerArea::UStandTriggerArea()
{
	//Enables tick
	PrimaryComponentTick.bCanEverTick = true;
}

void UStandTriggerArea::BeginPlay()
{
	Super::BeginPlay();

}

void UStandTriggerArea::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* puzzleActor = GetPuzzleActor();

	if (puzzleActor) 
	{
		UPrimitiveComponent* actorRootComponent = Cast<UPrimitiveComponent>(puzzleActor->GetRootComponent());

		if (actorRootComponent != nullptr)
		{
			actorRootComponent->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
			actorRootComponent->SetSimulatePhysics(false);
		}

		mover->SetCanMove(true);
		UE_LOG(LogTemp, Display, TEXT("Unlocking"));	
	}
	else 
	{
		mover->SetCanMove(false);
		UE_LOG(LogTemp, Display, TEXT("Relocking"))
	} 

}

void UStandTriggerArea::SetMover(UMover* newMover)
{
	mover = newMover;
}

//Get actor overlapping with trigger
AActor* UStandTriggerArea::GetPuzzleActor()
{
	//Get actors that overlap in a array
	TArray<AActor*> actors;
	GetOverlappingActors(actors);

	for (AActor* actor : actors)
	{	
		if (actor->ActorHasTag(puzzleTag) && !actor->ActorHasTag("Grabbed"))
		{
			//Return the actor
			return actor;
		}
	}

	//Return NULL if failed to get actor
	return nullptr;
}

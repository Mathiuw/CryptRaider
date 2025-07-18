// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetPhysicsHandleComponent()->GetGrabbedComponent() == nullptr) return;

	FVector targetLocation = GetComponentLocation() + GetForwardVector() * (SweepMaxDistance / 2);

	GetPhysicsHandleComponent()->SetTargetLocationAndRotation(targetLocation, GetComponentRotation());
}

void UGrabber::Grab()
{	
	FHitResult hitResult;

	if (GetGrabbableInReach(hitResult))
	{
		//DEBUG point
		DrawDebugPoint(GetWorld(), hitResult.ImpactPoint, 25, FColor::Red, false, 0.5);

		//Wake up and activate all rigidbodies
		UPrimitiveComponent* hitComponent = hitResult.GetComponent();
		hitComponent->SetSimulatePhysics(true);
		hitComponent->WakeAllRigidBodies();
		
		//Deatch from stand
		hitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		//Add "Grabbed" tag
		hitResult.GetActor()->Tags.Add("Grabbed");

		//Picks up the object
		GetPhysicsHandleComponent()->GrabComponentAtLocationWithRotation(
			hitComponent,
			NAME_None,
			hitResult.ImpactPoint,
			GetComponentRotation()
		);

		UE_LOG(LogTemp, Display, TEXT("Object hit: %s"), *hitResult.GetActor()->GetActorNameOrLabel());
	}
	else 
	{
		FVector end = GetComponentLocation() + GetForwardVector() * SweepMaxDistance;

		//DEBUG sphere
		DrawDebugSphere(GetWorld(), end, grabRadius, 16, FColor::Red, false, 0.5);
	
		UE_LOG(LogTemp, Display, TEXT("Hit nothing"));
	} 
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandleComponent();

	if (physicsHandle && GetPhysicsHandleComponent()->GetGrabbedComponent())
	{
		//Remove tag "Grabbed"
		UPrimitiveComponent* objectHeld = physicsHandle->GetGrabbedComponent();
		objectHeld->GetOwner()->Tags.Remove("Grabbed");

		physicsHandle->ReleaseComponent();

		UE_LOG(LogTemp, Display, TEXT("Released object grabbed"));
	}
	else UE_LOG(LogTemp, Display, TEXT("There's no object grabbed"));
}

bool UGrabber::GetGrabbableInReach(FHitResult& outHitResult) const
{
	FVector start = GetComponentLocation();
	FVector end = start + GetForwardVector() * SweepMaxDistance;
	FCollisionShape sphere = FCollisionShape::MakeSphere(grabRadius);

	return GetWorld()->SweepSingleByChannel(
		outHitResult,
		start, end,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		sphere
	);
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandleComponent() const
{
	UPhysicsHandleComponent* physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (physicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn doesnt have PhysicsHandleComponent"));
		return nullptr;
	}
	else return physicsHandle;
}

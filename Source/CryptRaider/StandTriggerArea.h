// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "StandTriggerArea.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UStandTriggerArea : public UBoxComponent
{
	GENERATED_BODY()

public:
	//Constructor
	UStandTriggerArea();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* mover);

private:

	UPROPERTY(EditAnywhere, Category = "Puzzle tag")
	FName puzzleTag;

	UMover* mover;

	AActor* GetPuzzleActor();

};

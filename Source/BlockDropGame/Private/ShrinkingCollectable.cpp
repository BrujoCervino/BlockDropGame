// Fill out your copyright notice in the Description page of Project Settings.


#include "ShrinkingCollectable.h"

AShrinkingCollectable::AShrinkingCollectable()
	:
	ShrinkAmount(0.01f)
{
}

void AShrinkingCollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector CurrentScale = GetActorScale3D();
	const FVector FinalScale = CurrentScale - (CurrentScale*ShrinkAmount);

	SetActorScale3D(FinalScale);

	// This variable should live inside the game mode: so it only exists once, but can be exposed to the editor
	const FVector SizeAtWhichDestroyed = FVector(FVector::OneVector*0.02);
	if (FinalScale.Equals(SizeAtWhichDestroyed))
	{
		Destroy();
		NotifyState(EGameState::EGS_MissedCollectable);
	}
}

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
	const float ZSizeAtWhichDestroyed = 0.1f;
	if ( FMath::IsNearlyEqual(FinalScale.Z, ZSizeAtWhichDestroyed, 0.05f ) )
	{
		Destroy();
		NotifyState(EGameState::EGS_MissedCollectable);
	}
}

const EGameState::Type AShrinkingCollectable::GetCollectedGameState() const
{
	return EGameState::EGS_CollectedRareCollectable;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectable.h"
#include "ShrinkingCollectable.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKDROPGAME_API AShrinkingCollectable : public ACollectable
{
	GENERATED_BODY()
	
public:

	AShrinkingCollectable();

	virtual void Tick(float DeltaTime) override;

protected:

	//~ Begin interface ACollectable
	virtual const EGameState::Type GetCollectedGameState() const override;
	//~ End interface ACollectable

private:

	// The amount per tick which this actor shrinks.
	// Should be positive.
	UPROPERTY(EditAnywhere)
	float ShrinkAmount;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlockDropPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKDROPGAME_API ABlockDropPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ABlockDropPlayerController();

	// Adds to the current score
	virtual void AddScore(const int32 ScoreToAdd);

protected:

	// The current score
	UPROPERTY(VisibleAnywhere)
	uint32 Score;
};

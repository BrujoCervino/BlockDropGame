// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameStateNotifier.h"
#include "BlockDropPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKDROPGAME_API ABlockDropPlayerController 
	: public APlayerController, public IGameStateNotifier
{
	GENERATED_BODY()
	
public:

	ABlockDropPlayerController();

	// Adds to the current score
	virtual void AddScore(const int32 ScoreToAdd);

protected:

	//~ Begin IGameStateNotifier Interface
	virtual void NotifyState(const EGameState::Type State) override;
	//~ End IGameStateNotifier Interface

	// The current score
	UPROPERTY(VisibleAnywhere)
	uint32 Score;
};

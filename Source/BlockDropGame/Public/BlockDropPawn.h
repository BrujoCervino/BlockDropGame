// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameStateNotifier.h"
#include "BlockDropPawn.generated.h"

class ABlockDropper;
class UCameraComponent;

UCLASS()
class BLOCKDROPGAME_API ABlockDropPawn 
	: public APawn, public IGameStateNotifier
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABlockDropPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Setter for BlockDropper
	virtual void SetBlockDropper(ABlockDropper* InBlockDopper);

protected:

	// Allows a block to begin falling
	virtual void ReleaseBlock();

	// Pause the game and show the pause widget
	void PauseGame();

	// Ask whether the player wants to quit before actually exiting
	bool RequestQuitGame();

	// Attemps to quit the game, asking the player first
	// Returns whether the quit was successful
	void QuitGame();

	//~ Begin IGameStateNotifier Interface
	virtual void NotifyState(const EGameState::Type State) override;
	//~ End IGameStateNotifier Interface

	// Returns BlockDropper
	ABlockDropper* GetBlockDropper() const;

private:

	// The block dropper whose actions this pawn's input affects
	UPROPERTY(Category="BlockDropGame", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	ABlockDropper* BlockDropper;

	// The camera attached to this player
	UPROPERTY(Category = "BlockDropGame", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	uint32 bPlayerRequestedQuitGame : 1;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameStateNotifier.h"
#include "BlockDropGameModeBase.generated.h"

class ABlockDropper;

/**
 * 
 */
UCLASS()
class BLOCKDROPGAME_API ABlockDropGameModeBase 
	: public AGameModeBase, public IGameStateNotifier
{
	GENERATED_BODY()

	ABlockDropGameModeBase();

protected:

	virtual void BeginPlay() override;

	//~ Begin IGameStateNotifier Interface
	virtual void NotifyState(const EGameState::Type State) override;
	//~ End IGameStateNotifier Interface

	// 
	virtual void RestartLevel();

	UPROPERTY(Category = "BlockDropGame", EditAnywhere)
	TSubclassOf<class ABlockDropper> BlockDropperClass;
	
	ABlockDropper* GetBlockDropper() const;

private:
	
	UPROPERTY(Category = "BlockDropGame", VisibleAnywhere)
	ABlockDropper* BlockDropper;

	UPROPERTY(Category = "BlockDropGame", EditAnywhere, BlueprintReadOnly, meta = ( AllowPrivateAccess = "true" ))
	float BlockDropperSpawnHeight;

	UPROPERTY(Category = "BlockDropGame", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FTimerHandle RestartLevelTimerHandle;

	UPROPERTY(Category = "BlockDropGame", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TimeSecondsUntilLevelRestarts;
};

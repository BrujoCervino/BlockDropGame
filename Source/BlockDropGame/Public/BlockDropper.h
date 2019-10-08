// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStateNotifier.h"
#include "BlockDropper.generated.h"

class ABlock;
class ACollectable;
class UCameraShake;
class UParticleSystemComponent;
class USoundCue;

UCLASS()
class BLOCKDROPGAME_API ABlockDropper 
	: public AActor, public IGameStateNotifier
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlockDropper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void HandleBlockHit();

	//~ Begin IGameStateNotifier Interface
	virtual void NotifyState(const EGameState::Type State) override;
	//~ End IGameStateNotifier Interface

	// Don't use this until I find a better way of doing it with fewer switch statements
	virtual void PlayBlockDropGameStateFX(const EGameState::Type State);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Lets the current block fall
	virtual void ReleaseBlock();

	// Returns whether
	virtual bool CurrentBlockIsFirstBlock(AActor const * const InBlock) const;

	// Not sure where to call this yet, but it will indicate whether all blocks should stop simulating physics
	virtual bool BlocksShouldStopSimulatingPhysics() const;

protected:  

	// The dynamic array of blocks spawned by this dropper
	UPROPERTY(VisibleAnywhere)
	TArray<ABlock*> SpawnedBlocks;

	// The current block being placed
	UPROPERTY(VisibleAnywhere)
	ABlock* CurrentBlock;

	// The particle used to indicate placing an object
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* PlacingParticle;

	// Material used when a block has been placed (move this to ABlock class).
	UPROPERTY(EditAnywhere)
	UMaterialInterface* SolidMaterial;

	// Material used when a block is being placed (move this to ABlock class).
	UPROPERTY(EditAnywhere)
	UMaterialInterface* GhostlyMaterial;

	// The sound played on scoring
	UPROPERTY(EditAnywhere)
	USoundCue* ScoredCue;

	// The sound played on scoring
	UPROPERTY(EditAnywhere)
	USoundCue* CollectedCue;

	// The sound played on failing (game over is the current fail state)
	UPROPERTY(EditAnywhere)
	USoundCue* FailedCue;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABlock> BlockClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACollectable> CommonCollectableClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACollectable> RareCollectableClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> ScoredCameraShake;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> CollectedCameraShake;

	// Amount this dropper rises after placing each block
	UPROPERTY(EditAnywhere)
	float HeightStep;

	// The height (above the highest place block) at which a collectable can spawn
	UPROPERTY(EditAnywhere)
	float CollectableSpawnHeight;

	// Speed (frequency) at which the block oscillates (move this to ABlock class).
	UPROPERTY(EditAnywhere)
	float BlockMoveSpeed;

	// Amplitude of the block's oscillation (move this to ABlock class).
	UPROPERTY(EditAnywhere)
	float BlockMoveAmplitude;

	// Whether or not this block dropper is at the starting phase of the state machine (maybe use enums for this instead?)
	UPROPERTY(VisibleAnywhere)
	uint32 bEntry : 1;

	// Whether we are awaiting player input (maybe rename to bAwaitingInput?)
	UPROPERTY(VisibleAnywhere)
	uint32 bPlayerChoosingDropPoint : 1;

	// Increments with score, so we can have build-up SFX
	UPROPERTY(VisibleAnywhere)
	int32 PitchMultiplier;
};

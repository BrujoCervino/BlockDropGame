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
	UPROPERTY(VisibleAnywhere, Category="Blocks")
	TArray<ABlock*> SpawnedBlocks;

	// The current block being placed
	UPROPERTY(VisibleAnywhere, Category = "Blocks")
	ABlock* CurrentBlock;

	// The particle used to indicate placing an object
	UPROPERTY(EditAnywhere, Category = "Blocks|Placement")
	UParticleSystemComponent* PlacingParticle;

	// Material used when a block has been placed (move this to ABlock class).
	UPROPERTY(EditAnywhere, Category = "Blocks")
	UMaterialInterface* SolidMaterial;

	// Material used when a block is being placed (move this to ABlock class).
	UPROPERTY(EditAnywhere, Category = "Blocks|Placement")
	UMaterialInterface* GhostlyMaterial;

	// The sound played on scoring
	UPROPERTY(EditAnywhere)
	USoundCue* ScoredCue;

	// The sound played on collecting a common collectable
	UPROPERTY(EditAnywhere)
	USoundCue* CommonCollectedCue;

	// The sound played on collecting a rare collectable
	UPROPERTY(EditAnywhere)
	USoundCue* RareCollectedCue;

	// The sound played on missing a rare collectable. Consider changing this to just a float with the pitch of -(RareCollectedCue->Pitch)
	UPROPERTY(EditAnywhere)
	USoundCue* RareMissedCue;

	// The sound played on failing (game over is the current fail state)
	UPROPERTY(EditAnywhere)
	USoundCue* FailedCue;
	
	UPROPERTY(EditAnywhere, Category = "Blocks")
	TSubclassOf<ABlock> BlockClass;

	UPROPERTY(EditAnywhere, Category = "Collectables")
	TSubclassOf<ACollectable> CommonCollectableClass;

	UPROPERTY(EditAnywhere, Category = "Collectables")
	TSubclassOf<ACollectable> RareCollectableClass;

	UPROPERTY(EditAnywhere, Category = "Blocks")
	TSubclassOf<UCameraShake> CommonBlockPlacedCameraShake;

	UPROPERTY(EditAnywhere, Category = "Collectables")
	TSubclassOf<UCameraShake> CommonCollectedCameraShake;

	// The chance of spawning a common collectable: gametime is moduloed by this then checked against zero.
	UPROPERTY(EditAnywhere, Category = "Collectables")
	float CommonCollectableSpawnChance;
	
	// The chance of spawning a common collectable: gametime is moduloed by this then checked against zero.
	UPROPERTY(EditAnywhere, Category = "Collectables")
	float RareCollectableSpawnChance;

	// Amount this dropper rises after placing each block
	UPROPERTY(EditAnywhere, Category = "Blocks|Placement")
	float HeightStep;

	// The height (above the highest place block) at which a collectable can spawn
	UPROPERTY(EditAnywhere, Category = "Collectables")
	float CollectableSpawnHeight;

	// Speed (frequency) at which the block oscillates (move this to ABlock class).
	UPROPERTY(EditAnywhere, Category = "Blocks|Placement")
	float BlockMoveSpeed;

	// Amplitude of the block's oscillation (move this to ABlock class).
	UPROPERTY(EditAnywhere, Category = "Blocks|Placement")
	float BlockMoveAmplitude;

	// Whether or not this block dropper is at the starting phase of the state machine (maybe use enums for this instead?)
	UPROPERTY(VisibleAnywhere, Category = "Blocks|Placement|State")
	uint32 bEntry : 1;

	// Whether we are awaiting player input (maybe rename to bAwaitingInput?)
	UPROPERTY(VisibleAnywhere, Category = "Blocks|Placement|State")
	uint32 bPlayerChoosingDropPoint : 1;

	// Increments with score, so we can have build-up SFX
	UPROPERTY(VisibleAnywhere)
	int32 PitchMultiplier;
};

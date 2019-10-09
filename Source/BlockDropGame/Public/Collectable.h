// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStateNotifier.h"
#include "Collectable.generated.h"

class USphereComponent;

/**
 *
 */
UCLASS()
class BLOCKDROPGAME_API ACollectable 
	: public AActor, public IGameStateNotifier
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectable();

	// Returns mesh
	UStaticMeshComponent* GetMesh() const;

	// Returns sphere trigger
	USphereComponent* GetSphereTrigger() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when this actor overlaps another actor, for example a player walking into a trigger.
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Called when another actor collects this collectable
	virtual void OnCollected(AActor* Collector);

	//~ Begin IGameStateNotifier Interface
	virtual void NotifyState(const EGameState::Type State) override;
	//~ End IGameStateNotifier Interface

	// The state sent by this collectable when collected
	virtual const EGameState::Type GetCollectedGameState() const;

	// The state sent by this collectable when missed
	virtual const EGameState::Type GetMissedGameState() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// The tag against which we'll check whether another class is a collectable
	static const FName CollectableTag;

protected:

	// The amount this actor spins (in worldspace) on each tick
	UPROPERTY(EditAnywhere)
	FRotator SpinAmount;

private:

	// The mesh representing with this collectable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	// The trigger whose overlap allows this actor's collection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereTrigger;

	// Whether this collectable has been picked up by the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint32 bCollected : 1;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDropGameModeBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Public/BlockDropPawn.h"
#include "Public/BlockDropper.h"

ABlockDropGameModeBase::ABlockDropGameModeBase()
{
	BlockDropperSpawnHeight = 20.0f;
}

ABlockDropper* ABlockDropGameModeBase::GetBlockDropper() const
{
	return BlockDropper;
}

void ABlockDropGameModeBase::BeginPlay()
{
	// If the current world is valid
	if (UWorld* const CurrentWorld = GetWorld())
	{
		// Make a default transform
		const FVector BlockDropperSpawnTranslation = FVector(0.0f, 0.0f, BlockDropperSpawnHeight);

		// Store this game mode as the owner of the BlockDropper, in case it needs to talk to this actor.
		FActorSpawnParameters BlockDropperSpawnParams;
		BlockDropperSpawnParams.Owner = this;
		
		// Chose UWorld::SpawnActor (rather than any implementation found within UGameplayStatics), because:
		// * I don't need this spawn to be Blueprint-friendly (it can still spawn Blueprint subclasses).
		// * I don't need to separately call the construction script.
		// * I want to specify the class of my spawned actor, so children won't need to cast it.
		BlockDropper = CurrentWorld->SpawnActor<ABlockDropper>(BlockDropperClass, BlockDropperSpawnTranslation, FRotator::ZeroRotator, BlockDropperSpawnParams);
	
		// Store a reference to our block dropper within the player
		ABlockDropPawn* const BlockDropPawn = Cast<ABlockDropPawn, APawn>( UGameplayStatics::GetPlayerPawn(this, 0) );
		if (BlockDropPawn) 
		{
			// Allow the pawn and the dropper to interact with one another:
			// Store a pointer to our BlockDropper within the BlockDropPawn
			BlockDropPawn->SetBlockDropper(BlockDropper);
			// Store a pointer (of class AActor) to BlockDropper within BlockDropPawn
			BlockDropper->SetOwner(BlockDropPawn);
		}
	}
}

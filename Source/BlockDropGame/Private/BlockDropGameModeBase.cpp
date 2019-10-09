// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDropGameModeBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Public/BlockDropPawn.h"
#include "Public/BlockDropper.h" // For cleaner code, this should just be done inside the pawn
#include "TimerManager.h"

ABlockDropGameModeBase::ABlockDropGameModeBase()
	:
	BlockDropper(nullptr),
	BlockDropperSpawnHeight(20.0f),
	RestartLevelTimerHandle(),
	TimeSecondsUntilLevelRestarts(0.5f),
	PointsPerCommonBlock(5),
	PointsPerCommonCollectable(20),
	PointsPerRareCollectable(40),
	PointsPerMissedCollectable(0)
{
}

int32 ABlockDropGameModeBase::GetScorePerGameState(const EGameState::Type State)
{
	int32 Ret = 0;
	
	switch (State)
	{
		case(EGameState::EGS_GameOver):
		{
			RestartLevel();
			break;
		}
		case(EGameState::EGS_PlacedCommonBlock):
		{
			Ret = PointsPerCommonBlock;
			break;
		}
		case(EGameState::EGS_CollectedCommonCollectable):
		{
			Ret = PointsPerCommonCollectable;
			break;
		}
		case(EGameState::EGS_CollectedRareCollectable):
		{
			Ret = PointsPerRareCollectable;
			break;
		}
		case(EGameState::EGS_MissedCollectable):
		{
			Ret = PointsPerMissedCollectable;
			break;
		}
		default:
		{
			// This should remain unreachable
			checkNoEntry();
			break;
		}
	}
	
	return Ret;
}

ABlockDropper* ABlockDropGameModeBase::GetBlockDropper() const
{
	return BlockDropper;
}

void ABlockDropGameModeBase::BeginPlay()
{
	Super::BeginPlay();

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
			// Finally, allow the pawn to interact with this game mode:
			BlockDropPawn->SetOwner(this);
		}
	}
}

void ABlockDropGameModeBase::NotifyState(const EGameState::Type State)
{
	switch (State) 
	{
		case(EGameState::EGS_GameOver):
		{
			RestartLevel();
			break;
		}
		case(EGameState::EGS_PlacedCommonBlock):
		{
			break;
		}
		case(EGameState::EGS_CollectedCommonCollectable):
		{
			break;
		}
		case(EGameState::EGS_CollectedRareCollectable):
		{
			break;
		}
		case(EGameState::EGS_MissedCollectable):
		{
			break;
		}
		default:
		{
			// This should remain unreachable
			checkNoEntry();
			break;
		}
	}
}

void ABlockDropGameModeBase::RestartLevel()
{
	// After TimeSecondsUntilLevelRestarts, restart the level
	if(APlayerController* const PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		GetWorldTimerManager().SetTimer(RestartLevelTimerHandle, PC, &APlayerController::RestartLevel, TimeSecondsUntilLevelRestarts);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDropper.h"
#include "Collectable.h"
#include "Components/StaticMeshComponent.h" // Note: I can rid of this by making ABlock::SetMaterial and ABlock::SetPhysicsState
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Public/Block.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

// Sets default values
ABlockDropper::ABlockDropper()
	:
	SpawnedBlocks(nullptr, 0),
	CurrentBlock(nullptr),
	PlacingParticle(CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PlacingParticle"))),
	SolidMaterial(nullptr),
	GhostlyMaterial(nullptr),
	ScoredCue(nullptr),
	CollectedCue(nullptr),
	FailedCue(nullptr),
	BlockClass(ABlock::StaticClass()),
	CommonCollectableClass(ACollectable::StaticClass()),
	RareCollectableClass(ACollectable::StaticClass()),
	ScoredCameraShake(),
	CollectedCameraShake(),
	HeightStep(20.0f),	
	CollectableSpawnHeight(60.0f),
	BlockMoveSpeed(30.0f),			  
	BlockMoveAmplitude(200.0f),		  
	bEntry(true),					  
	bPlayerChoosingDropPoint(true),
	PitchMultiplier(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// If the particle exists (has been set in the editor)
	if (PlacingParticle)
	{
		// Let it be the root of this actor
		SetRootComponent(PlacingParticle);
	}
}

// Called when the game starts or when spawned
void ABlockDropper::BeginPlay()
{
	Super::BeginPlay();

}

void ABlockDropper::HandleBlockHit()
{
	// (Don't null-check CurrentBlock, because this function is called inside it).
	// If the current block is not the first block spawned,
	if (SpawnedBlocks.Last() != nullptr)
	{
		// Check whether the current block is atop SpawnedBlocks.Last()
	}
	else
	{
		// Check whether the current block is touching the floor
	}
}

void ABlockDropper::NotifyState(const EGameState::Type State)
{
	// Declare the FX played per game state:
	USoundBase* SoundToPlay = nullptr;
	TSubclassOf<UCameraShake> ShakeToPlay = TSubclassOf<UCameraShake>();

	switch (State) 
	{
		case (EGameState::EGS_GameOver):
		{
			SoundToPlay = FailedCue;
			break;
		}
		case (EGameState::EGS_Scored):
		{
			SoundToPlay = ScoredCue;
			ShakeToPlay = ScoredCameraShake;
			// Increase the pitch of our sound
			++PitchMultiplier;
			break;
		}
		case (EGameState::EGS_CollectedCollectable):
		{
			SoundToPlay = CollectedCue;
			ShakeToPlay = CollectedCameraShake;
			// Increase the pitch of our sound more than usual, to show the collectable is an extra special way of earning points
			PitchMultiplier += (int32)EGameState::EGS_CollectedCollectable;
			break;
		}
		default:
		{
			// The switch should never be able to reach this point
			checkNoEntry();
			break;
		}
	};

	// Play the sound correspondent to our new game state
	UGameplayStatics::PlaySound2D(this, SoundToPlay);
	
	if (APlayerController* const PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->ClientPlayCameraShake(ShakeToPlay);
	}

	// Tell the game mode we scored
	if (IGameStateNotifier* const Notifier = Cast<IGameStateNotifier, AActor>(GetOwner()))
	{
		Notifier->NotifyState(State);
	}
}

void ABlockDropper::PlayBlockDropGameStateFX(const EGameState::Type State)
{
	
}

// Called every frame
void ABlockDropper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bEntry)
	{
		// Move up
		const FVector HeightToAdd = FVector::UpVector * HeightStep;
		AddActorWorldOffset(HeightToAdd);				  // This actor moves up to stop blocks spawning in the same place
		if (GetOwner())
		{
			GetOwner()->AddActorWorldOffset(HeightToAdd); // The player moves up, allowing the camera to move up with it
		}
		
		// If the CurrentWorld is valid,
		if ( UWorld* const CurrentWorld = GetWorld() )
		{
			// If the current block exists, 
			if (CurrentBlock)
			{				
				// Add the current block to the array of spawned blocks.
				SpawnedBlocks.Add(CurrentBlock);

				// Prepare the current block pointer to store the next block
				CurrentBlock = nullptr;
			}

			// Spawn a block
			FActorSpawnParameters BlockSpawnParams;
			BlockSpawnParams.Owner = this;

			CurrentBlock = CurrentWorld->SpawnActor<ABlock>(BlockClass, GetActorTransform(), BlockSpawnParams);

			// Use a transparent material to show the block is not ready to interact with the other blocks yet
			CurrentBlock->GetMesh()->SetMaterial(0, GhostlyMaterial);

			// Attach the placing particle to the current block
			PlacingParticle->AttachToComponent(CurrentBlock->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);

			// Every so often (and if the topmost block is valid)
			if (FMath::RandRange(0, 3) == 0 && SpawnedBlocks.Num() != 0)
			{
				// Print a string to confirm out collectable appeared (for if we can't see it)
				UKismetSystemLibrary::PrintString(this, TEXT("Collectable placed!"));
				
				// Determine whether the class to spawn will be common or rare :)
				UClass* const ClassToSpawn = FMath::RandRange(0,2) == 0? RareCollectableClass : CommonCollectableClass;

				FTransform TransformOfHighestPlacedBlock = SpawnedBlocks.Last()->GetActorTransform();
				TransformOfHighestPlacedBlock += FTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f, CollectableSpawnHeight), FVector::ZeroVector);
				
				// Spawn a collectable atop the topmost block
				CurrentWorld->SpawnActor<ACollectable>(ClassToSpawn, TransformOfHighestPlacedBlock, BlockSpawnParams);
			}

			// To avoid unfair gameplay, every few blocks stacked will stop simulating physics. 
			// This means offscreen/very low blocks' positions can't decrease the stability of the tower
			const int Moduloer = 3;
			if (BlocksShouldStopSimulatingPhysics() && SpawnedBlocks.Num() % Moduloer == 0)
			{
				for (auto It = SpawnedBlocks.CreateConstIterator(); It; ++It)
				{
					(*It)->GetMesh()->SetSimulatePhysics(false);	
				}
			}

		}
		// We're no longer at entry point
		bEntry = false;
	}
	else if (bPlayerChoosingDropPoint)
	{
		if (CurrentBlock) 
		{
			// Oscillate the block left to right
			FVector FinalPosition = GetActorLocation();

			const float a = GetWorld()->TimeSeconds * BlockMoveSpeed;
			const float b = FMath::Sin(a) * BlockMoveAmplitude;

			FinalPosition.Y = b;

			// Move the block
			CurrentBlock->SetActorLocation(FinalPosition);
		}
	}
	else
	{
		// Show the player the block is now "solid"
		CurrentBlock->GetMesh()->SetMaterial(0, SolidMaterial);
		// Allow the block to collide
		CurrentBlock->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		// Drop the block
		CurrentBlock->GetMesh()->SetSimulatePhysics(true);
		// Restart the loop :)
		bEntry = true;
		bPlayerChoosingDropPoint = true;
	}
}

void ABlockDropper::ReleaseBlock()
{
	bPlayerChoosingDropPoint = false;
}

bool ABlockDropper::CurrentBlockIsFirstBlock(AActor const * const InBlock ) const
{
	const FString FirstBlockName = GetDebugName(SpawnedBlocks[0]);
	const FString InBlockName = GetDebugName(InBlock);

	return FirstBlockName.Equals(InBlockName, ESearchCase::CaseSensitive);
}

bool ABlockDropper::BlocksShouldStopSimulatingPhysics() const
{
	return (SpawnedBlocks.Num() != 0 && SpawnedBlocks.Last()->ShouldStopSimulatingPhysics());
}


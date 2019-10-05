// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockDropper.h"
#include "Components/StaticMeshComponent.h" // Note: I can rid of this by making ABlock::SetMaterial
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Block.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
ABlockDropper::ABlockDropper()
	:
	SpawnedBlocks(nullptr, 0),		  
	CurrentBlock(nullptr),
	PlacingParticle(CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PlacingParticle"))),
	SolidMaterial(nullptr),			  
	GhostlyMaterial(nullptr),	
	ScoredCue(nullptr),
	FailedCue(nullptr),
	BlockClass(ABlock::StaticClass()),
	HeightStep(20.0f),				  
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

void ABlockDropper::NotifyScored()
{
	if (ScoredCue)
	{
		++PitchMultiplier;

		// Use pitch multiplier to have 5 different pitches for scoring
		
		UGameplayStatics::PlaySound2D(this, ScoredCue);
	}
	// Tell the player controller we scored
	
}

void ABlockDropper::NotifyFailed()
{
	// Play a sound to indicate failure
	UGameplayStatics::PlaySound2D(this, FailedCue);



	// Restart the current level
	const FName LevelName = *UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, LevelName);
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

void ABlockDropper::NotifyState(const EGameState::Type)
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

			// To avoid unfair gameplay, every few blocks stacked will stop simulating physics. 
			// This means offscreen/very low blocks' positions can't decrease the stability of the tower
			const int Moduloer = 3;
			if (SpawnedBlocks.Num() != 0 && SpawnedBlocks.Num() % Moduloer == 0)
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

bool ABlockDropper::CurrentBlockIsFirstBlock() const
{
	return (SpawnedBlocks.Num() == 1);
}


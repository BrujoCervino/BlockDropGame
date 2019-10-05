// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "Components/StaticMeshComponent.h"
#include "Public/BlockDropper.h"

// Sets default values
ABlock::ABlock()
	:
	Mesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"))),
	bScoreNotified(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetRootComponent( Mesh );
	Mesh->SetMobility(EComponentMobility::Movable);			 // A block moves, so we can't bake its lighting
	Mesh->SetSimulatePhysics(false);						 // A block shouldn't fall on spawn
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // A block should not collide on spawn, though it should call overlap events.
	Mesh->SetNotifyRigidBodyCollision(true);	// Call ABlock::NotifyHit whenever this block hits or is hit by another rigid body.

	Tags.AddUnique(BlockTag); // Add the block tag, so other blocks know this is a block
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
}

void ABlock::NotifyScored()
{
	// If this block hasn't already called NotifyScored,
	if (!GetScoreNotified())
	{
		// Tell the block dropper we scored
		ABlockDropper* const BlockDropper = GetOwningBlockDropper();
		if (BlockDropper)
		{
			BlockDropper->NotifyScored();
			
			// Disallow this block from calling this function more than once.
			bScoreNotified = true;
		}
	}
}

void ABlock::NotifyFailed()
{
	// Tell the block dropper we scored
	ABlockDropper* const BlockDropper = GetOwningBlockDropper();
	if (BlockDropper)
	{
		BlockDropper->NotifyFailed();
	}
}

ABlockDropper* ABlock::GetOwningBlockDropper() const
{
	return Cast<ABlockDropper, AActor>( GetOwner() );
}

bool ABlock::GetScoreNotified() const
{
	return bScoreNotified;
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetVelocity() != FVector::ZeroVector)
	{
		NotifyScored();
		SetActorTickEnabled(false);
	}
}

void ABlock::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	// If the other actor is also a block,
	if (Other->ActorHasTag(BlockTag))		
	{
		// Begin ticking, so we can check velocity
		SetActorTickEnabled(true);
		GetMesh()->SetNotifyRigidBodyCollision(false);
	}
	// Elsewise, if the this block is the first ever spawned block,
	else if (GetOwningBlockDropper()->CurrentBlockIsFirstBlock())
	{
		// Allow the first falling block to touch the ground
		NotifyScored();
		GetMesh()->SetSimulatePhysics(false);
	}
	else
	{
		// Restart the level
		NotifyFailed();
	}
}

const FName ABlock::BlockTag = FName("Block");
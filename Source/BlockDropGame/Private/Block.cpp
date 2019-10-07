// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "Components/StaticMeshComponent.h"
#include "Public/BlockDropper.h"

// Sets default values
ABlock::ABlock()
	:
	Mesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"))),
	bHasSentNotification(false)
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

bool ABlock::ShouldStopSimulatingPhysics() const
{
	return (bHasSentNotification && GetVelocity().IsNearlyZero());
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
}

void ABlock::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	// If the other actor is also a block,
	if (Other->ActorHasTag(BlockTag) || GetOwningBlockDropper()->CurrentBlockIsFirstBlock(this))
	{
		// Begin ticking, so we can check velocity
		SetActorTickEnabled(true);
		GetMesh()->SetNotifyRigidBodyCollision(false);
	}
	// Else, we must have failed.
	else
	{
		NotifyState(EGameState::EGS_GameOver);
	}
}

ABlockDropper* ABlock::GetOwningBlockDropper() const
{
	return Cast<ABlockDropper, AActor>( GetOwner() );
}

bool ABlock::HasSentNotification() const
{
	return bHasSentNotification;
}

void ABlock::NotifyState(const EGameState::Type State)
{
	// If this block actor hasn't already called NotifyScored or NotifyFailed,
	if (!HasSentNotification())
	{
		// Tell the block dropper we scored
		if ( IGameStateNotifier* const Notifier = Cast<IGameStateNotifier, AActor>( GetOwner() ) )
		{
			Notifier->NotifyState(State);
		}
		// Disallow this block from calling this function more than once.
		bHasSentNotification = true;
	}
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetVelocity() != FVector::ZeroVector)
	{
		const EGameState::Type RAND_STATE_TEMP = FMath::RandBool() ? EGameState::EGS_Scored : EGameState::EGS_SecondaryThing;


		NotifyState(/*EGameState::EGS_Scored*/ RAND_STATE_TEMP);
		SetActorTickEnabled(false);
	}
}

const FName ABlock::BlockTag = FName("Block");
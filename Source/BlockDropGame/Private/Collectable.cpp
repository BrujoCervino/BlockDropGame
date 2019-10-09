// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable.h"
#include "BlockDropper.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ACollectable::ACollectable()
	:
	SpinAmount(0.0f, 5.0f, 0.0f),
	Mesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"))),
	SphereTrigger(CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"))),
	bCollected(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	if (SphereTrigger)
	{
		// Let the sphere trigger be the root, so we can transform the mesh without affecting the core functionality
		SetRootComponent(SphereTrigger);

	}
	if (Mesh)
	{
		Mesh->SetupAttachment(SphereTrigger);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

UStaticMeshComponent* ACollectable::GetMesh() const
{
	return Mesh;
}

USphereComponent* ACollectable::GetSphereTrigger() const
{
	return SphereTrigger;
}

// Called when the game starts or when spawned
void ACollectable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Mesh->AddWorldRotation(SpinAmount);
}

const FName ACollectable::CollectableTag = "Collectable";

void ACollectable::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (!bCollected)
	{
		OnCollected(OtherActor);
		
		// This mesh can only be collected once
		bCollected = true;
	}
}

void ACollectable::OnCollected(AActor* Collector)
{
	// Let other actors know this actor has been collected.
	NotifyState(GetCollectedGameState());	

	// Hide the actor, then destroy it.
	SetActorHiddenInGame(true);
	Destroy();
}

void ACollectable::NotifyState(const EGameState::Type State)
{
	// Tell the block dropper we sscored
	if (IGameStateNotifier* const Notifier = Cast<IGameStateNotifier, AActor>(GetOwner()))
	{
		Notifier->NotifyState(State);
	}
}

const EGameState::Type ACollectable::GetCollectedGameState() const
{
	return EGameState::EGS_CollectedCommonCollectable;
}

const EGameState::Type ACollectable::GetMissedGameState() const
{
	return EGameState::EGS_MissedCollectable;
}


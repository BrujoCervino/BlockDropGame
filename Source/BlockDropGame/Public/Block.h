// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

class ABlockDropper;

UCLASS()
class BLOCKDROPGAME_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlock();

	// Returns the mesh subobject.
	inline UStaticMeshComponent* GetMesh() const { return Mesh; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Tell other actors that we scored
	virtual void NotifyScored();

	// Tell other actors we failed
	virtual void NotifyFailed();

	// (Maybe move all activity with ABlockDropper to happen within ABlockDropper)
	ABlockDropper* GetOwningBlockDropper() const;

	bool GetScoreNotified() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when this actor hits, or is hit by, another actor
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, 
		FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	// The tag against which we'll check whether another class is a block
	static const FName BlockTag;

private:

	// The mesh which represents both the physics and appearance of this block.
	UPROPERTY(Category="BlockDropGame", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Mesh;

	// Whether this block has notified other actors it has contributed to the player's score.
	// Stops the game from spamming FX 
	UPROPERTY(Category = "BlockDropGame", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint32 bScoreNotified :1;
};

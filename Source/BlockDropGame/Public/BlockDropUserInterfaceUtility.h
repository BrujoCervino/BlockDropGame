// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BlockDropUserInterfaceUtility.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UBlockDropUserInterfaceUtility : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BLOCKDROPGAME_API IBlockDropUserInterfaceUtility
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Adds to the currently displayed score
	UFUNCTION(BlueprintNativeEvent)
	void AddScore(const int32 InScore);
	virtual void AddScore_Implementation(const int32 InScore) = 0;

	// Updates the current score: used when the score 
	UFUNCTION(BlueprintNativeEvent)
	void RefreshScore(const int32 InScore);
	virtual void RefreshScore_Implementation(const int32 InScore) = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HumanPlayer.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ChessPlayerController.generated.h"

// event to spawn the replay buttons
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonSpawn);
// event to spanw the promotion menu
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPromotion);
/**
 * 
 */
UCLASS()
class PAA_CHESSPROJECT_API AChessPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "ButtonEvent");
	FButtonSpawn SpawnButtonEvent;

	UPROPERTY(BlueprintAssignable, Category = "ButtonEvent");
	FOnPromotion PromoteEvent;

	AChessPlayerController();

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* ChessContext;


	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ClickAction;

	void ClickOnGrid();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

};

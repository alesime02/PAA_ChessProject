// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerInterface.h"
#include "ChessGameInstance.h"
#include "ChessGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "RandomPlayer.generated.h"

UCLASS()
class PAA_CHESSPROJECT_API ARandomPlayer : public APawn, public IPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARandomPlayer();

	UChessGameInstance* GameInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn() override;
	virtual void OnWin() override;
	//virtual void OnLose() override;
};

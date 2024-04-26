// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ChessGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PAA_CHESSPROJECT_API UChessGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FString MoveMessage;

	UPROPERTY(EditAnywhere)
	FString CurrentField;

	UPROPERTY(EditAnywhere)
	int32 ScoreHumanPlayer = 0;

	// score value for AI player
	UPROPERTY(EditAnywhere)
	int32 ScoreAiPlayer = 0;

	// message to show every turn
	UPROPERTY(EditAnywhere)
	FString CurrentTurnMessage = "Current Player";

	// increment the score for human player
	void IncrementScoreHumanPlayer();

	// increment the score for AI player
	void IncrementScoreAiPlayer();

	// get the score for human player
	UFUNCTION(BlueprintCallable)
	int32 GetScoreHumanPlayer();

	// get the score for AI player
	UFUNCTION(BlueprintCallable)
	int32 GetScoreAiPlayer();

	// get the current turn message
	UFUNCTION(BlueprintCallable)
	FString GetTurnMessage();

	// set the turn message
	UFUNCTION(BlueprintCallable)
	void SetTurnMessage(FString Message);

	UFUNCTION(BlueprintCallable)
	void SetMessage(FString Msg) { MoveMessage = Msg; }

	UFUNCTION(BlueprintCallable)
	FString GetMessage() { return MoveMessage; }

	UFUNCTION(BlueprintCallable)
	void SetField(FString Fld) { CurrentField = Fld; }

	UFUNCTION(BlueprintCallable)
	FString GetField() { return CurrentField; }
	
};

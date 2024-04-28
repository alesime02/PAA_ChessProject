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
	// the string that will appear on the replay buttons
	UPROPERTY(EditAnywhere)
	FString MoveMessage;

	// the button associated string that describe the field after the move they dispay
	UPROPERTY(EditAnywhere)
	FString CurrentField;

	// score value for human player
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

	// set the move message
	UFUNCTION(BlueprintCallable)
	void SetMessage(FString Msg) { MoveMessage = Msg; }

	// get the move message
	UFUNCTION(BlueprintCallable)
	FString GetMessage() { return MoveMessage; }

	//  set the field string
	UFUNCTION(BlueprintCallable)
	void SetField(FString Fld) { CurrentField = Fld; }

	// get the field string
	UFUNCTION(BlueprintCallable)
	FString GetField() { return CurrentField; }
};

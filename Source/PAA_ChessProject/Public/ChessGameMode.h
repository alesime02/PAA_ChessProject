// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInterface.h"
#include "GameField.h"
#include "GameFramework/GameModeBase.h"		
#include "ChessGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PAA_CHESSPROJECT_API AChessGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	// function that create the string that will appear on the replay buttons
	void CreateCurrentMove(ATile* Start, ATile* End, APiece* Moving, TCHAR Case);

	// function that create the string that represent the field after a move
	void CreateFieldStatus();
	
	// array of players
	TArray<IPlayerInterface*> Players;

	// identifier of the player that has to play the turn
	int32 CurrentPlayer;

	// function called in blueprint to show at screen the current turn message
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentPlayer() { return CurrentPlayer; }

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGameField> GameFieldClass;

	// reference to a GameField object
	UPROPERTY(VisibleAnywhere)
	AGameField* GField;

	// boolean to determine the end of the game
	UPROPERTY(Transient)
	bool CheckMate = false;	

	// boolean to determine the end of the game
	UPROPERTY(Transient)
	bool Pair = false;

	AChessGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// called at the start of the game
	void ChoosePlayerAndStartGame();

	// get the next player index
	int32 GetNextPlayer(int32 Player);

	// called at the end of the game turn
	void TurnNextPlayer();

	// call the Current's PossibleMoves function than the FilterIllegals Function to let in the moves array
	// of the piece only the legal moves then mark them with green
	void LegalMoves(APiece* Current);

	// check if a move is legal or not
	void FilterIllegals(APiece* Current);

	// check if there is a checkmate
	void IsCheck(APiece* Current, AChessKing* EnemyKing, TArray<APiece*> EnemyPieces);

	// check if the game is ended with a pair situation
	void IsPair(TArray<APiece*> EnemyPieces);

	// restore the original material of the tiles
	void DecoloringTiles();
};


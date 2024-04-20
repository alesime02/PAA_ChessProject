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
	
	TArray<IPlayerInterface*> Players;

	int32 CurrentPlayer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGameField> GameFieldClass;

	// reference to a GameField object
	UPROPERTY(VisibleAnywhere)
	AGameField* GField;

	UPROPERTY(Transient)
	bool CheckMate = false;	

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

	void LegalMoves(APiece* Current);

	void FilterIllegals(APiece* Current);

	void IsCheck(APiece* Current, AChessKing* EnemyKing, TArray<APiece*> EnemyPieces);

	void IsPair(TArray<APiece*> EnemyPieces);
};

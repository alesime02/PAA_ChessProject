// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "ChessBishop.h"
#include "ChessRook.h"
#include "ChessQueen.h"
#include "ChessKing.h"
#include "ChessKnight.h"
#include "ChessPawn.h"
#include "GameFramework/Actor.h"
#include "GameField.generated.h"

UCLASS()
class PAA_CHESSPROJECT_API AGameField : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameField();

	// keeps track of tiles
	UPROPERTY(Transient)
	TArray<ATile*> TileArray;

	// given a position returns a tile
	UPROPERTY(Transient)
	TMap<FVector2D, ATile*> TileMap;

	// keep track of the white pieces in game
	UPROPERTY(Transient)
	TArray<APiece*> WPieceInGame;

	// keep track of the black pieces in game
	UPROPERTY(Transient)
	TArray<APiece*> BPieceInGame;

	// a pointer to the black King in game
	UPROPERTY(Transient)
	AChessKing* BlackKing;

	// a pointer to the white King in game
	UPROPERTY(Transient)
	AChessKing* WhiteKing;

	// function to modifie the padding between the tiles(in this project is 0)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float NormalizedCellPadding;

	// size of field
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Size;

	// pointer to the pawn that has to be promoted
	APiece* ReceivingPromotion;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessRook> RookClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessBishop> BishopClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessKing> KingClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessQueen> QueenClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessKnight> KnightClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessPawn> PawnClass;

	// tile padding dimension
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CellPadding;

	// tile size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TileSize;

	// piece size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PieceSize;

	// the string that represent the field that has to be generated
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString FieldAtStart;

	// Called when an instance of this class is placed (in editor) or spawned
	virtual void OnConstruction(const FTransform& Transform) override;

	// function used to promote the black pawns
	void RandomBlackPromotion();

	// functions used to let the human player choose the pawn promotion
	UFUNCTION(BlueprintCallable)
	void PromotePawnToQueen();

	UFUNCTION(BlueprintCallable)
	void PromotePawnToRook();

	UFUNCTION(BlueprintCallable)
	void PromotePawnToBishop();

	UFUNCTION(BlueprintCallable)
	void PromotePawnToKnight();

	// restore the field to the initial status
	UFUNCTION(BlueprintCallable)
	void ResetField();

	// recreate the field of a specific move
	UFUNCTION(BlueprintCallable)
	void ReplayField(FString FieldToReturn, int32 Player);

	// generate an empty game field
	void GenerateField();

	// generate the pieces
	UFUNCTION(BlueprintCallable)
	void SpawnPieces(FString Field);

	// return a (x,y) position given a hit (click) on a field tile
	FVector2D GetPosition(const FHitResult& Hit);

	// return the array of tile pointers
	TArray<ATile*>& GetTileArray();

	// return a relative position given (x,y) position of a tile
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;

	// return a relative tile position given (x,y) position of a piece
	FVector GetPieceRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;

	// return (x,y) piece position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location) const;

	// functions to spawn teh white and black pieces in a templated way
	void SpawnBlackPiece(APiece* ToSpawn, FString MaterialPath, int32 DestX, int32 DestY, TCHAR id, UStaticMeshComponent* Component);

	void SpawnWhitePiece(APiece* ToSpawn, int32 DestX, int32 DestY, TCHAR id);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};

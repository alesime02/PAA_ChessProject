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


// macro declaration for a dynamic multicast delegate
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReset);


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

	//given a position returns a tile
	UPROPERTY(Transient)
	TMap<FVector2D, ATile*> TileMap;

	UPROPERTY(Transient)
	TArray<APiece*> WPieceInGame;

	UPROPERTY(Transient)
	TArray<APiece*> BPieceInGame;

	UPROPERTY(Transient)
	APiece* BlackKing;

	UPROPERTY(Transient)
	APiece* WhiteKing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float NormalizedCellPadding;

	static const int32 NOT_ASSIGNED = -1;

	// BlueprintAssignable Usable with Multicast Delegates only. Exposes the property for assigning in Blueprints.
	// declare a variable of type FOnReset (delegate)
	//UPROPERTY(BlueprintAssignable)
	//FOnReset OnResetEvent;

	// size of field
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Size;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PieceSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString FieldStatus;

	// Called when an instance of this class is placed (in editor) or spawned
	virtual void OnConstruction(const FTransform& Transform) override;

	// remove all signs from the field
	UFUNCTION(BlueprintCallable)
	void ResetField();

	// generate an empty game field
	void GenerateField();

	// genera le pedine
	void SpawnPawns();

	//restituisce la stringa di stato
	FString& GetStatus();

	// return a (x,y) position given a hit (click) on a field tile
	FVector2D GetPosition(const FHitResult& Hit);

	// return the array of tile pointers
	TArray<ATile*>& GetTileArray();

	// return a relative position given (x,y) position of a tile
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;

	// return a relative position given (x,y) position of a piece
	FVector GetPieceRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;

	// return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location) const;





protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};

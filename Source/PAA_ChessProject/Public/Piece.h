// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "Piece.generated.h"

class AGameField;

UCLASS()
class PAA_CHESSPROJECT_API APiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APiece();

	// (x, y) position of the tile, aggiunto per i test
	FVector2D PieceGridPosition;

	// array of the legal moves
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FVector2D> Moves;

	int32 BitColor = 0;

	virtual void PossibleMoves(AGameField* Field) {};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};

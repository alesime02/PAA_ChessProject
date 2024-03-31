// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "GameFramework/Actor.h"
#include "ChessRook.generated.h"

UCLASS()
class PAA_CHESSPROJECT_API AChessRook : public APiece
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessRook();

	UStaticMeshComponent* GetStatMeshComp();//aggiunto per i test

	/*// get the tile owner
	int32 GetColor();

	//cambia il colore della pedina
	void ChangeBitColor();

	// set the (x, y) position, aggiunto per i test
	void SetGridPosition(const double InX, const double InY);

	// get the (x, y) position, aggiunto per i test
	FVector2D GetGridPosition();*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*// (x, y) position of the tile, aggiunto per i test
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D RookGridPosition;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 BitColor;*/

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};

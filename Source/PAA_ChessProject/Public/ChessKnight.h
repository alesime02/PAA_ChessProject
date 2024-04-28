// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "GameFramework/Actor.h"
#include "ChessKnight.generated.h"

UCLASS()
class PAA_CHESSPROJECT_API AChessKnight : public APiece
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessKnight();

	UStaticMeshComponent* GetStatMeshComp();

	// override of the function to compute the possible moves
	virtual void PossibleMoves(AGameField* Field) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};

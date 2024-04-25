// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPawn.h"
#include "GameField.h"

// Sets default values
AChessPawn::AChessPawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);
}

UStaticMeshComponent* AChessPawn::GetStatMeshComp()
{
	return StaticMeshComponent;
}

void AChessPawn::PossibleMoves(AGameField* Field)
{
	Moves.Empty();
	const double StartX = this->PieceGridPosition[0];
	const double StartY = this->PieceGridPosition[1];
	if (this->BitColor == 0 && StartX < 7) 
	{
		FVector2D temp(StartX + 1, StartY);
		ATile* Next = Field->TileMap[(temp)];
		if (Next->GetTileStatus() == EStatus::EMPTY)
		{
			Moves.Add(temp);
			if (StartX == 1)
			{
				temp.X += 1;
				temp.Y = StartY;
				Next = Field->TileMap[(temp)];
				if (Next->GetTileStatus() == EStatus::EMPTY)
				{
					Moves.Add(temp);
					temp.X -= 1;
				}
			}
		}
		if (StartY > 0)
		{
			temp.Y = StartY - 1;
			ATile* NextLDiagonal = Field->TileMap[(temp)];
			if (NextLDiagonal->GetTileStatus() == EStatus::BLACKOCCUPIED)
			{
				Moves.Add(temp);

			}
		}
		if (StartY < 7)
		{
			temp.Y = StartY + 1;
			ATile* NextRDiagonal = Field->TileMap[(temp)];
			if (NextRDiagonal->GetTileStatus() == EStatus::BLACKOCCUPIED)
			{
				Moves.Add(temp);
			}
		}
	}
	else if (this->BitColor == 1 && StartX > 0)
	{
		FVector2D temp(StartX - 1, StartY);
		ATile* Next = Field->TileMap[(temp)];
		if (Next->GetTileStatus() == EStatus::EMPTY)
		{
			Moves.Add(temp);
			if (StartX == 6)
			{
				temp.X -= 1;
				temp.Y = StartY;
				Next = Field->TileMap[(temp)];
				if (Next->GetTileStatus() == EStatus::EMPTY)
				{
					Moves.Add(temp);
				}
			}
		}
		if (StartY > 0)
		{
			temp.Y = StartY - 1;
			ATile* NextLDiagonal = Field->TileMap[(temp)];
			if (NextLDiagonal->GetTileStatus() == EStatus::WHITEOCCUPIED)
			{
				Moves.Add(temp);
			}
		}
		if (StartY < 7)
		{
			temp.Y = StartY + 1;
			ATile* NextRDiagonal = Field->TileMap[(temp)];
			if (NextRDiagonal->GetTileStatus() == EStatus::WHITEOCCUPIED)
			{
				Moves.Add(temp);
			}
		}
	}
}



// Called when the game starts or when spawned
void AChessPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*void AChessPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/


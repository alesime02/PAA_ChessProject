// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameMode.h"
#include "ChessPlayerController.h"
#include "HumanPlayer.h"
#include "RandomPlayer.h"
//#include "TTT_MinimaxPlayer.h"
#include "EngineUtils.h"


AChessGameMode::AChessGameMode()
{
	PlayerControllerClass = AChessPlayerController::StaticClass();
	DefaultPawnClass = AHumanPlayer::StaticClass();
}

void AChessGameMode::BeginPlay()
{
	Super::BeginPlay();

	IsGameOver = false;

	AHumanPlayer* HumanPlayer = Cast<AHumanPlayer>(*TActorIterator<AHumanPlayer>(GetWorld()));
	
	int32 FieldSize;
	if (GameFieldClass != nullptr)
	{
		GField = GetWorld()->SpawnActor<AGameField>(GameFieldClass);
		FieldSize = GField->Size;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
	}

	float CameraPosX = ((GField->TileSize * (FieldSize + ((FieldSize - 1) * GField->NormalizedCellPadding) - (FieldSize - 1))) / 2) - (GField->TileSize / 2);
	FVector CameraPos(CameraPosX, CameraPosX, 1000.0f);
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

	// Human player = 0
	Players.Add(HumanPlayer);
	// Random Player
	auto* AI = GetWorld()->SpawnActor<ARandomPlayer>(FVector(), FRotator());

	// MiniMax Player
	//auto* AI = GetWorld()->SpawnActor<ATTT_MinimaxPlayer>(FVector(), FRotator());

	// AI player = 1
	Players.Add(AI);

	this->ChoosePlayerAndStartGame();
}

void AChessGameMode::ChoosePlayerAndStartGame()
{
	CurrentPlayer = 0;

	for (int32 i = 0; i < Players.Num(); i++)
	{
		Players[i]->PlayerNumber = i;
	}
	Players[CurrentPlayer]->OnTurn();
}

int32 AChessGameMode::GetNextPlayer(int32 Player)
{
	Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;
}

void AChessGameMode::TurnNextPlayer()
{
	CurrentPlayer = GetNextPlayer(CurrentPlayer);
	Players[CurrentPlayer]->OnTurn();
}

void AChessGameMode::FilterIllegals(APiece* Current)
{
	if (Current->BitColor == 0) 
	{
		FVector2D KingPosition;
		for (int32 i = 0; i < Current->Moves.Num(); i++) 
		{
			if (Current != GField->WhiteKing) 
			{
				KingPosition = GField->WhiteKing->PieceGridPosition;
			}
			else
			{
				KingPosition = Current->Moves[i];
			}
			ATile* Start = GField->TileMap[(Current->PieceGridPosition)];
			Start->SetTileStatus(ETileStatus::EMPTY);
			ATile* End = GField->TileMap[(Current->Moves[i])];
			ETileStatus Previous = End->GetTileStatus();
			End->SetTileStatus(ETileStatus::WHITEOCCUPIED);
			for (int32 j = 0; j < GField->BPieceInGame.Num(); j++) 
			{
				APiece* Curr = GField->BPieceInGame[j];
				Curr->PossibleMoves(GField);
				for (int32 k = 0; k < Curr->Moves.Num(); k++) 
				{
					if (Curr->Moves[k] == KingPosition) 
					{
						Current->Moves.RemoveAt(i);
					}
				}
				Start->SetTileStatus(ETileStatus::WHITEOCCUPIED);
				End->SetTileStatus(Previous);
			}
		}
	}
	else
	{
		FVector2D KingPosition;
		for (int32 i = 0; i < Current->Moves.Num(); i++)
		{
			if (Current != GField->BlackKing)
			{
				KingPosition = GField->BlackKing->PieceGridPosition;
			}
			else
			{
				KingPosition = Current->Moves[i];
			}
			ATile* Start = GField->TileMap[(Current->PieceGridPosition)];
			Start->SetTileStatus(ETileStatus::EMPTY);
			ATile* End = GField->TileMap[(Current->Moves[i])];
			ETileStatus Previous = End->GetTileStatus();
			End->SetTileStatus(ETileStatus::BLACKOCCUPIED);
			for (int32 j = 0; j < GField->WPieceInGame.Num(); j++)
			{
				APiece* Curr = GField->WPieceInGame[j];
				Curr->PossibleMoves(GField);
				for (int32 k = 0; k < Curr->Moves.Num(); k++)
				{
					if (Curr->Moves[k] == KingPosition)
					{
						Current->Moves.RemoveAt(i);
					}
				}
				Start->SetTileStatus(ETileStatus::BLACKOCCUPIED);
				End->SetTileStatus(Previous);
			}
		}
	}
}


void AChessGameMode::LegalMoves(APiece* Current)
{
	Current->PossibleMoves(GField);
	FilterIllegals(Current);
	for (int32 i = 0; i < Current->Moves.Num(); i++) 
	{
		ATile* LegalTile = GField->TileMap[(Current->Moves[i])];
		FString MaterialPath = TEXT("/Game/Materials/MI_LegalMove");
		UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
		UStaticMeshComponent* Comp = LegalTile->GetStatMeshComp();
		Comp->SetMaterial(0, Material);
	}
}


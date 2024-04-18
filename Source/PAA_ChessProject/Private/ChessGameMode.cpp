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
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("CHIAMATAFUNZIONE"));
	//	FVector2D KingPosition;
	//	EStatus EnemyStatus;
	//	EStatus YourStatus;
	//	TArray<APiece*> EnemyPieces;
	//
	//	if (Current->BitColor == 0)
	//	{
	//		KingPosition = GField->WhiteKing->PieceGridPosition;
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Preso posizione re"));
	//		EnemyStatus = EStatus::BLACKOCCUPIED;
	//		YourStatus = EStatus::WHITEOCCUPIED;
	//		EnemyPieces = GField->BPieceInGame;
	//	}
	//	else
	//	{
	//		KingPosition = GField->BlackKing->PieceGridPosition;
	//		EnemyStatus = EStatus::WHITEOCCUPIED;
	//		YourStatus = EStatus::BLACKOCCUPIED;
	//		EnemyPieces = GField->WPieceInGame;
	//	}
	//	bool MovingKing = false;
	//	if (Current->PieceGridPosition == KingPosition)
	//	{
	//		MovingKing = true;
	//	}
	//	TArray<FVector2D> ActualMoves = Current->Moves;
	//	FVector2D ToNotCount(-1, -1);
	//	// scorro le mosse possibili della pedina che ho cliccato
	//	for (auto i : Current->Moves)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Scorro Mosse Pedina cliccata"));
	//		ATile* Start = GField->TileMap[Current->PieceGridPosition];
	//		//simulo la mossa
	//		if (MovingKing)
	//		{
	//			KingPosition = i;
	//		}
	//		Start->SetTileStatus(EStatus::EMPTY);
	//		ATile* End = GField->TileMap[i];
	//		EStatus Previous = End->GetTileStatus();
	//		if (Previous == EnemyStatus)
	//		{
	//			ToNotCount = End->GetGridPosition();
	//		}
	//		End->SetTileStatus(YourStatus);
	//		//calcolo contromosse dei pezzi nemici
	//		bool loopBreaker = false;
	//		for (auto Piece : GField->WPieceInGame)
	//		{
	//			if (loopBreaker)
	//			{
	//				break;
	//			}
	//			if (Piece->PieceGridPosition == ToNotCount) continue;
	//	
	//			Piece->PossibleMoves(GField);
	//			for (auto EnemyMove : Piece->Moves)
	//			{
	//				if (EnemyMove == KingPosition)
	//				{
	//					ActualMoves.Remove(i);
	//					loopBreaker = true;
	//					break;
	//				}
	//			}
	//	
	//		}
	//		Start->SetTileStatus(YourStatus);
	//		End->SetTileStatus(Previous);
	//		ToNotCount.X = -1;
	//		ToNotCount.Y = -1;
	//	}
	//	Current->Moves = ActualMoves;
	//}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("CHIAMATAFUNZIONE"));
	if (Current->BitColor == 0)
	{
		FVector2D KingPosition = GField->WhiteKing->PieceGridPosition;
		bool MovingKing = 0;
		if (Current->PieceGridPosition == KingPosition)
		{
			MovingKing = 1;
		}
		TArray<FVector2D> ActualMoves = Current->Moves;
		FVector2D ToNotCount(-1, -1);
		// scorro le mosse possibili della pedina che ho cliccato
		for (auto i : Current->Moves)
		{
			ATile* Start = GField->TileMap[Current->PieceGridPosition];
			//simulo la mossa
			if (MovingKing)
			{
				KingPosition = i;
			}
			Start->SetTileStatus(EStatus::EMPTY);
			ATile* End = GField->TileMap[i];
			EStatus Previous = End->GetTileStatus();
			if (Previous == EStatus::BLACKOCCUPIED)
			{
				ToNotCount = End->GetGridPosition();
			}
			End->SetTileStatus(EStatus::WHITEOCCUPIED);
			//calcolo contromosse dei pezzi nemici
			bool loopBreaker = false;
			for (auto Piece : GField->BPieceInGame)
			{
				if (loopBreaker)
				{
					break;
				}
				if (Piece->PieceGridPosition == ToNotCount)
				{
					continue;
				}
				Piece->PossibleMoves(GField);
				for (auto EnemyMove : Piece->Moves)
				{
					if (EnemyMove == KingPosition)
					{
						ActualMoves.Remove(i);
						loopBreaker = true;
						break;
					}
				}

			}
			Start->SetTileStatus(EStatus::WHITEOCCUPIED);
			End->SetTileStatus(Previous);
			ToNotCount.X = -1;
			ToNotCount.Y = -1;
		}
		Current->Moves = ActualMoves;
	}

	else
	{
		FVector2D KingPosition = GField->BlackKing->PieceGridPosition;
		bool MovingKing = 0;
		if (Current->PieceGridPosition == KingPosition)
		{
			MovingKing = 1;
		}
		TArray<FVector2D> ActualMoves = Current->Moves;
		FVector2D ToNotCount(-1, -1);
		// scorro le mosse possibili della pedina che ho cliccato
		for (auto i : Current->Moves)
		{
			ATile* Start = GField->TileMap[Current->PieceGridPosition];
			//simulo la mossa
			if (MovingKing)
			{
				KingPosition = i;
			}
			Start->SetTileStatus(EStatus::EMPTY);
			ATile* End = GField->TileMap[i];
			EStatus Previous = End->GetTileStatus();
			if (Previous == EStatus::WHITEOCCUPIED)
			{
				ToNotCount = End->GetGridPosition();
			}
			End->SetTileStatus(EStatus::BLACKOCCUPIED);
			//calcolo contromosse dei pezzi nemici
			bool loopBreaker = false;
			for (auto Piece : GField->WPieceInGame)
			{
				if (loopBreaker)
				{
					break;
				}
				if (Piece->PieceGridPosition == ToNotCount)
				{
					continue;
				}
				Piece->PossibleMoves(GField);
				for (auto EnemyMove : Piece->Moves)
				{
					if (EnemyMove == KingPosition)
					{
						ActualMoves.Remove(i);
						loopBreaker = true;
						break;
					}
				}

			}
			Start->SetTileStatus(EStatus::BLACKOCCUPIED);
			End->SetTileStatus(Previous);
			ToNotCount.X = -1;
			ToNotCount.Y = -1;
		}
		Current->Moves = ActualMoves;
	}
}



	//if (Current->BitColor == 0) 
	//{
	//	FVector2D KingPosition;
	//	FVector2D ToNotCount;
	//	//APiece* Eaten = nullptr;
	//	int32 MovingKing = 0;
	//	TArray<FVector2D> ActualMoves = Current->Moves;
	//	int32 Iterator = 0;
	//	for (int32 i = 0; i < Current->Moves.Num(); i++) 
	//	{
	//		if (Current != GField->WhiteKing) 
	//		{
	//			KingPosition = GField->WhiteKing->PieceGridPosition;
	//		}
	//		else
	//		{
	//			KingPosition = Current->Moves[i];
	//			MovingKing = 1;
	//		}
	//		ATile* Start = GField->TileMap[(Current->PieceGridPosition)];
	//		Start->SetTileStatus(EStatus::EMPTY);
	//		ATile* End = GField->TileMap[(Current->Moves[i])];
	//		EStatus Previous = End->GetTileStatus();
	//		if (Previous == EStatus::BLACKOCCUPIED)
	//		{
	//			 FVector2D EndXY = End->GetGridPosition();
	//			 ToNotCount.X = EndXY.X;
	//			 ToNotCount.Y = EndXY.Y;
	//			//Eaten = GField->PieceMap[(End->GetGridPosition())];
	//			/*if (GField->PieceMap.Find(End->GetGridPosition()) != nullptr) 
	//			{
	//				Eaten = GField->PieceMap[(End->GetGridPosition())];
	//			}*/
	//		}
	//		End->SetTileStatus(EStatus::WHITEOCCUPIED);
	//		for (int32 j = 0; j < GField->BPieceInGame.Num(); j++) 
	//		{
	//			if (GField->BPieceInGame[j]->PieceGridPosition == ToNotCount) 
	//			{
	//				continue;
	//			}
	//			/*if (Eaten != nullptr) 
	//			{
	//				if (GField->BPieceInGame[j] == Eaten)
	//				{
	//					continue;
	//				}
	//			}*/
	//			else 
	//			{
	//				APiece* Curr = GField->BPieceInGame[j];
	//				Curr->PossibleMoves(GField);
	//				for (int32 k = 0; k < Curr->Moves.Num(); k++)
	//				{
	//					if (Curr->Moves[k] == KingPosition)
	//					{
	//						ActualMoves.RemoveAt(Iterator);
	// 
	//						break;
	//					}
	//					else 
	//					{
	//						Iterator += 1;
	//					}
	//				}
	//				Start->SetTileStatus(EStatus::WHITEOCCUPIED);
	//				End->SetTileStatus(Previous);
	//				ToNotCount.X = Current->PieceGridPosition.X;
	//				ToNotCount.Y = Current->PieceGridPosition.Y;
	//				if (MovingKing == 1) 
	//				{
	//					KingPosition = GField->WhiteKing->PieceGridPosition;
	//				}
	//			}
	//				
	//		}
	//	}
	//}
	/*else
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
			Start->SetTileStatus(EStatus::EMPTY);
			ATile* End = GField->TileMap[(Current->Moves[i])];
			EStatus Previous = End->GetTileStatus();
			End->SetTileStatus(EStatus::BLACKOCCUPIED);
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
				Start->SetTileStatus(EStatus::BLACKOCCUPIED);
				End->SetTileStatus(Previous);
			}
		}
	}*/
//}


void AChessGameMode::LegalMoves(APiece* Current)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("CHIAMATA PossibleMoves"));
	Current->PossibleMoves(GField);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("CHIAMATA Filterillegals"));
	FilterIllegals(Current);
	for (auto i : Current->Moves) 
	{
		ATile* LegalTile = GField->TileMap[i];
		FString MaterialPath = TEXT("/Game/Materials/MI_LegalMove");
		UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(NULL, nullptr, *MaterialPath));
		UStaticMeshComponent* Comp = LegalTile->GetStatMeshComp();
		Comp->SetMaterial(0, Material);
	}
}


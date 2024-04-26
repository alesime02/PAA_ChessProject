// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomPlayer.h"
#include "ChessPlayerController.h"

// Sets default values
ARandomPlayer::ARandomPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

// Called when the game starts or when spawned
void ARandomPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARandomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARandomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARandomPlayer::OnTurn()
{
	GameInstance->SetTurnMessage(TEXT("AI (Random) Turn"));
	Working = true;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{

		AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
		int32 RandPiece = FMath::Rand() % GameMode->GField->BPieceInGame.Num();
		GameMode->LegalMoves(GameMode->GField->BPieceInGame[RandPiece]);
		for (auto PieceIdx : GameMode->GField->BPieceInGame)
		{
			if (GameMode->GField->BPieceInGame[RandPiece]->Moves.IsEmpty())
			{
				RandPiece = (RandPiece + 1) % GameMode->GField->BPieceInGame.Num();
				GameMode->LegalMoves(GameMode->GField->BPieceInGame[RandPiece]);
			}
			else
			{
				break;
			}
		}
		APiece* MovingPiece = GameMode->GField->BPieceInGame[RandPiece];
		int32 RandMove = FMath::Rand() % GameMode->GField->BPieceInGame[RandPiece]->Moves.Num();
		ATile* Start = GameMode->GField->TileMap[MovingPiece->PieceGridPosition];
		Start->SetTileStatus(EStatus::EMPTY);
		ATile* End = GameMode->GField->TileMap[MovingPiece->Moves[RandMove]];
		if (End->GetTileStatus() == EStatus::EMPTY)
		{
			End->SetTileStatus(EStatus::BLACKOCCUPIED);
			int32 x = End->GetGridPosition().X;
			int32 y = End->GetGridPosition().Y;
			FVector WhereToGo = GameMode->GField->GetPieceRelativeLocationByXYPosition(x, y);
			MovingPiece->SetActorLocation(WhereToGo);
			MovingPiece->PieceGridPosition = GameMode->GField->GetXYPositionByRelativeLocation(WhereToGo);
			Start->SetOccupier(' ');
			End->SetOccupier(MovingPiece->Id);
			GameMode->CreateCurrentMove(Start, End, MovingPiece, '-');
			GameMode->CreateFieldStatus();
			GameMode->IsPair(GameMode->GField->WPieceInGame);
			GameMode->IsCheck(MovingPiece, GameMode->GField->WhiteKing, GameMode->GField->WPieceInGame);
			auto PC = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
			if (PC)
			{
				PC->SpawnButtonEvent.Broadcast();
			}
			GameMode->DecoloringTiles();
			GameMode->TurnNextPlayer();
		}
		else
		{
			End->SetTileStatus(EStatus::BLACKOCCUPIED);
			TArray<APiece*> Copy = GameMode->GField->WPieceInGame;
			for (auto ToKill : Copy)
			{
				if (ToKill->PieceGridPosition == End->GetGridPosition())
				{
					GameMode->GField->WPieceInGame.Remove(ToKill);
					ToKill->Destroy();
					break;
				}
			}
			int32 x = End->GetGridPosition().X;
			int32 y = End->GetGridPosition().Y;
			FVector WhereToGo = GameMode->GField->GetPieceRelativeLocationByXYPosition(x, y);
			MovingPiece->SetActorLocation(WhereToGo);
			MovingPiece->PieceGridPosition = GameMode->GField->GetXYPositionByRelativeLocation(WhereToGo);
			Start->SetOccupier(' ');
			End->SetOccupier(MovingPiece->Id);
			GameMode->CreateCurrentMove(Start, End, MovingPiece, 'x');
			GameMode->CreateFieldStatus();
			GameMode->IsPair(GameMode->GField->WPieceInGame);
			GameMode->IsCheck(MovingPiece, GameMode->GField->WhiteKing, GameMode->GField->WPieceInGame);
			auto PC = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
			if (PC)
			{
				PC->SpawnButtonEvent.Broadcast();
			}
			GameMode->DecoloringTiles();
			GameMode->TurnNextPlayer();
		}
	
	}, 2, false);
	Working = false;
}

void ARandomPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Wins!"));
	GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	GameInstance->IncrementScoreAiPlayer();
}

//void ARandomPlayer::OnLose()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Loses!"));
//	GameInstance->SetTurnMessage(TEXT("AI Loses!"));
//}


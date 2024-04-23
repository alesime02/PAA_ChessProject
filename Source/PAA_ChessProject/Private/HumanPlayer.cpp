// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanPlayer.h"

// Sets default values
AHumanPlayer::AHumanPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// create a camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//set the camera as RootComponent
	SetRootComponent(Camera);
	// get the game instance reference
	GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	// default init values
	PlayerNumber = -1;
}

// Called when the game starts or when spawned
void AHumanPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHumanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHumanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHumanPlayer::OnTurn()
{
	IsMyTurn = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Your Turn"));
	GameInstance->SetTurnMessage(TEXT("Human Turn"));
}

void AHumanPlayer::OnWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Win!"));
	GameInstance->SetTurnMessage(TEXT("Human Wins!"));
	GameInstance->IncrementScoreHumanPlayer();
}

void AHumanPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Lose!"));
	GameInstance->SetTurnMessage(TEXT("Human Loses!"));
}

void AHumanPlayer::OnClick()
{
	
	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (Hit.bBlockingHit && IsMyTurn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Tocco"));
		AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
		if (APiece* CurrPiece = Cast<APiece>(Hit.GetActor()))
		{
			if (CurrPiece->BitColor == 0)
			{
				//AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
				if (ClickBit) 
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Decoloring Tiles"));
					GameMode->DecoloringTiles();
					ClickBit = false;
				}
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("calcola mosse"));
				GameMode->LegalMoves(CurrPiece);
				PieceToMove = CurrPiece;
				ClickBit = true;
			}
		}
		if (ATile* MoveTo = Cast<ATile>(Hit.GetActor()))
		{
			//AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
			if (PieceToMove != nullptr) 
			{
				for (auto iterator : PieceToMove->Moves) 
				{
					if (MoveTo->GetGridPosition() == iterator) 
					{
						ATile* Start = GameMode->GField->TileMap[PieceToMove->PieceGridPosition];
						Start->SetTileStatus(EStatus::EMPTY);
						MoveTo->SetTileStatus(EStatus::WHITEOCCUPIED);
						int32 x = MoveTo->GetGridPosition().X;
						int32 y = MoveTo->GetGridPosition().Y;
						FVector WhereToGo = GameMode->GField->GetPieceRelativeLocationByXYPosition(x,y);
						PieceToMove->SetActorLocation(WhereToGo);
						PieceToMove->PieceGridPosition = GameMode->GField->GetXYPositionByRelativeLocation(WhereToGo);
						GameMode->DecoloringTiles();
						GameMode->IsPair(GameMode->GField->BPieceInGame);
						GameMode->IsCheck(PieceToMove, GameMode->GField->BlackKing, GameMode->GField->BPieceInGame);
						IsMyTurn = false;
						GameMode->TurnNextPlayer();
					}
				}
			}
		}
		if (APiece* ToCapture = Cast<APiece>(Hit.GetActor()))
		{
			//AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
			if(PieceToMove != nullptr && ToCapture->BitColor == 1)
			{
				ATile* MoveTo = GameMode->GField->TileMap[ToCapture->PieceGridPosition];
				for (auto iterator : PieceToMove->Moves) 
				{
					if (MoveTo->GetGridPosition() == iterator) 
					{
						ATile* Start = GameMode->GField->TileMap[PieceToMove->PieceGridPosition];
						Start->SetTileStatus(EStatus::EMPTY);
						MoveTo->SetTileStatus(EStatus::WHITEOCCUPIED);
						TArray<APiece*> Copy = GameMode->GField->BPieceInGame;
						for (auto ToKill : Copy)
						{
							if (ToKill->PieceGridPosition == MoveTo->GetGridPosition())
							{
								GameMode->GField->BPieceInGame.Remove(ToKill);
								ToKill->Destroy();
								break;
							}
						}
						int32 x = MoveTo->GetGridPosition().X;
						int32 y = MoveTo->GetGridPosition().Y;
						FVector WhereToGo = GameMode->GField->GetPieceRelativeLocationByXYPosition(x, y);
						PieceToMove->SetActorLocation(WhereToGo);
						PieceToMove->PieceGridPosition = GameMode->GField->GetXYPositionByRelativeLocation(WhereToGo);
						GameMode->DecoloringTiles();
						GameMode->IsPair(GameMode->GField->BPieceInGame);
						GameMode->IsCheck(PieceToMove, GameMode->GField->BlackKing, GameMode->GField->BPieceInGame);
						IsMyTurn = false;
						GameMode->TurnNextPlayer();
					}
				}
			}
		}
	}
}


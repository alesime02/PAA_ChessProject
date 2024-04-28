// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanPlayer.h"
#include "ChessPlayerController.h"

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

// commented because redundant
//void AHumanPlayer::OnLose()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Lose!"));
//	GameInstance->SetTurnMessage(TEXT("Human Loses!"));
//}

void AHumanPlayer::OnClick()
{
	
	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	// click to select the piece to move
	if (Hit.bBlockingHit && IsMyTurn)
	{
		AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
		if (APiece* CurrPiece = Cast<APiece>(Hit.GetActor()))
		{
			if (CurrPiece->BitColor == 0)
			{
				if (ClickBit) 
				{
					GameMode->DecoloringTiles();
					ClickBit = false;
				}
				// compute the legal moves and mark them with the color green
				GameMode->LegalMoves(CurrPiece);
				PieceToMove = CurrPiece;
				ClickBit = true;
			}
		}
		// click for moving the piece on a free tile
		if (ATile* MoveTo = Cast<ATile>(Hit.GetActor()))
		{
			if (PieceToMove != nullptr) 
			{
				// checking if the tile clicked is a legal move
				TArray<FVector2D> CopyOfMoves = PieceToMove->Moves;
				for (auto iterator : CopyOfMoves) 
				{
					if (MoveTo->GetGridPosition() == iterator) 
					{
						// moving the piece and updating all the properties of field and piece that are involved
						ATile* Start = GameMode->GField->TileMap[PieceToMove->PieceGridPosition];
						Start->SetTileStatus(EStatus::EMPTY);
						MoveTo->SetTileStatus(EStatus::WHITEOCCUPIED);
						int32 x = MoveTo->GetGridPosition().X;
						int32 y = MoveTo->GetGridPosition().Y;
						FVector WhereToGo = GameMode->GField->GetPieceRelativeLocationByXYPosition(x,y);
						PieceToMove->SetActorLocation(WhereToGo);
						PieceToMove->PieceGridPosition = GameMode->GField->GetXYPositionByRelativeLocation(WhereToGo);
						// in case the moved piece was a pawn and has now to be promoted
						if (PieceToMove->Id == 'P' && MoveTo->GetGridPosition().X == 7)
						{
							Start->SetOccupier(' ');
							GameMode->GField->ReceivingPromotion = PieceToMove;
							GameMode->CreateCurrentMove(Start, MoveTo, PieceToMove, '-');
							auto PC = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
							if (PC)
							{
								// event that let the player choose the promotion
								PC->PromoteEvent.Broadcast();
							}
							IsMyTurn = false;
						}
						
						else
						{
							Start->SetOccupier(' ');
							MoveTo->SetOccupier(PieceToMove->Id);
							GameMode->DecoloringTiles();
							GameMode->CreateCurrentMove(Start, MoveTo, PieceToMove, '-');
							GameMode->CreateFieldStatus();
							GameMode->IsPair(GameMode->GField->BPieceInGame);
							GameMode->IsCheck(PieceToMove, GameMode->GField->BlackKing, GameMode->GField->BPieceInGame);
							auto PC = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
							if (PC)
							{
								// event that create the button for the replay
								PC->SpawnButtonEvent.Broadcast();
							}

							IsMyTurn = false;
							GameMode->TurnNextPlayer();
						}
					}
				}
			}
			
		}
		// click for moving the piece on an occupied tile
		if (APiece* ToCapture = Cast<APiece>(Hit.GetActor()))
		{
			// same check did it for the move on a free tile
			if(PieceToMove != nullptr && ToCapture->BitColor == 1)
			{
				ATile* MoveTo = GameMode->GField->TileMap[ToCapture->PieceGridPosition];
				TArray<FVector2D> CopyOfMoves = PieceToMove->Moves;
				for (auto iterator : CopyOfMoves) 
				{
					if (MoveTo->GetGridPosition() == iterator) 
					{
						ATile* Start = GameMode->GField->TileMap[PieceToMove->PieceGridPosition];
						Start->SetTileStatus(EStatus::EMPTY);
						MoveTo->SetTileStatus(EStatus::WHITEOCCUPIED);
						TArray<APiece*> Copy = GameMode->GField->BPieceInGame;
						// removing the captured piece
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
						// in case the moved piece was a pawn and has now to be promoted
						if (PieceToMove->Id == 'P' && MoveTo->GetGridPosition().X == 7)
						{
							Start->SetOccupier(' ');
							GameMode->GField->ReceivingPromotion = PieceToMove;
							GameMode->CreateCurrentMove(Start, MoveTo, PieceToMove, 'x');
							auto PC = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
							if (PC)
							{
								PC->PromoteEvent.Broadcast();
							}
							IsMyTurn = false;
						}
						else
						{
							Start->SetOccupier(' ');
							MoveTo->SetOccupier(PieceToMove->Id);
							GameMode->DecoloringTiles();
							GameMode->CreateCurrentMove(Start, MoveTo, PieceToMove, 'x');
							GameMode->CreateFieldStatus();
							GameMode->IsPair(GameMode->GField->BPieceInGame);
							GameMode->IsCheck(PieceToMove, GameMode->GField->BlackKing, GameMode->GField->BPieceInGame);
							auto PC = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
							if (PC)
							{
								PC->SpawnButtonEvent.Broadcast();
							}
							IsMyTurn = false;
							GameMode->TurnNextPlayer();
						}
					}
				}
			}
		}
	}
}


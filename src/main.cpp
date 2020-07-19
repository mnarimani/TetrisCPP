
#include <iostream>

#include "Renderer.h"
#include "PlayGround.h"
#include "ScoreDrawer.h"
#include "Tetromino.h"
#include "thread"

static bool isRotateHold = false;
static bool isLeftHold = false;
static bool isRightHold = false;
static bool inputKeys[4];

/// <summary>
/// Reads input from console and moves the passed tetromino in playground
/// </summary>
/// <param name="playGround">The playground that tetromino lives in</param>
/// <param name="tetromino">Tetromino to move</param>
void ReadInput(const std::shared_ptr<PlayGround>& playGround, const std::shared_ptr<Tetromino>& tetromino)
{
	for (int k = 0; k < 4; k++)                                // R   L   D Z
	{
		inputKeys[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
	}

	// Move piece to target position. If it fits, keep it there, if not, reset

	if (inputKeys[0])
	{
		if (isRightHold == false)
		{
			tetromino->SetPositionX(tetromino->PositionX() + 1);
			if (playGround->CheckPiecePlacement(tetromino) == false)
			{
				tetromino->SetPositionX(tetromino->PositionX() - 1);
			}
			else
			{
				isRightHold = true;
			}
		}
	}
	else
	{
		isRightHold = false;
	}

	if (inputKeys[1])
	{
		if (isLeftHold == false)
		{
			tetromino->SetPositionX(tetromino->PositionX() - 1);
			if (playGround->CheckPiecePlacement(tetromino) == false)
			{
				tetromino->SetPositionX(tetromino->PositionX() + 1);
			}
			else
			{
				isLeftHold = true;
			}
		}
	}
	else
	{
		isLeftHold = false;
	}

	if (inputKeys[2])
	{
		tetromino->SetPositionY(tetromino->PositionY() + 1);
		if (playGround->CheckPiecePlacement(tetromino) == false)
		{
			tetromino->SetPositionY(tetromino->PositionY() - 1);
		}
	}

	if (inputKeys[3])
	{
		if (isRotateHold == false)
		{
			tetromino->SetRotation(tetromino->Rotation() + 1);
			if (playGround->CheckPiecePlacement(tetromino) == false)
			{
				tetromino->SetRotation(tetromino->Rotation() - 1);
			}
			else
			{
				isRotateHold = true;
			}
		}
	}
	else
	{
		isRotateHold = false;
	}
}

/// <summary>
/// Starts one round of the game and returns final score.
/// </summary>
/// <returns>Score</returns>
int PlayGame()
{
	const int tickRate = 60;
	const int deltaTime = 1000 / tickRate;

	Renderer renderer(80, 30);

	auto playGround = std::make_shared<PlayGround>(12, 18);
	renderer.AddDrawable(playGround);

	auto scoreDrawer = std::make_shared<ScoreDrawer>();
	renderer.AddDrawable(scoreDrawer);

	std::shared_ptr<Tetromino> currentTetromino = nullptr;

	// Move down every half second
	int moveDownTimeThreshold = 400;
	int moveDownTimer = 0;

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(deltaTime));

		// Spawn a piece if we don't have it
		if (currentTetromino == nullptr)
		{
			currentTetromino = Tetromino::Create(playGround->Width() / 2, 0, 'A');
			renderer.AddDrawable(currentTetromino);

			if (playGround->CheckPiecePlacement(currentTetromino) == false)
			{
				// Lost the game.
				break;
			}
		}

		ReadInput(playGround, currentTetromino);

		// We want to move tetromino down automatically every N milliseconds. N is defined as moveDownTimeThreshold
		moveDownTimer += deltaTime;
		if (moveDownTimer > moveDownTimeThreshold)
		{
			currentTetromino->SetPositionY(currentTetromino->PositionY() + 1);

			if (playGround->CheckPiecePlacement(currentTetromino) == false)
			{
				// Cannot move piece there
				// Revert back and lock it in place
				currentTetromino->SetPositionY(currentTetromino->PositionY() - 1);
				const int constructedLines = playGround->LockPiece(currentTetromino);

				renderer.RemoveDrawable(currentTetromino);
				// Assigning tetromino to nullptr will generate a new one at next frame.
				currentTetromino = nullptr;

				scoreDrawer->Score += (1 << constructedLines) * 10;
			}

			moveDownTimer = 0;
		}

		renderer.DrawAll();
	}

	return scoreDrawer->Score;
}

int main()
{
	const int score = PlayGame();
	std::cout << "Game Over. Score: " << score << std::endl;
	std::cin.get();
}

#pragma once
#include "game.h"

bool IsThreefoldRepetition(Game& game);
unsigned char GetGameState(Game& game); //TODO: add threefold repetition and insufficient material
int EvaluateMaterial(Game& game);
int EvaluatePosition(Game& game);
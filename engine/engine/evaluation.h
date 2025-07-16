#pragma once
#include "game.h"

unsigned char GetGameState(Game& game); //TODO: add threefold repetition and insufficient material
int EvaluateMaterial(Game& game);
int EvaluatePosition(Game& game);
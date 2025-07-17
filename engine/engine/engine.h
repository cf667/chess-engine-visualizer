#pragma once
#include <queue>

#include "game.h"

extern std::queue<std::string> msgQueue;
extern std::mutex msgQueueMutex;
extern std::condition_variable msgQueueReady;

void MessageHandler(std::string msg, Game& game);

int EngineThread();
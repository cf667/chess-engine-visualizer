#pragma once
#include <queue>
#include <mutex>

#include "game.h"

extern std::queue<std::string> msgQueue;
extern std::mutex msgQueueMutex;
extern std::condition_variable msgQueueReady;

void SocketMessageHandler(std::string msg, Game& game);

bool UCIMessageHandler(std::string message, Game& game);

int EngineThread(const bool useUCI);
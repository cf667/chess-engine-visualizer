#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <queue>
#include <mutex>

#include "game.h"

struct EngineSettings
{
	bool connectedToGUI = false;

	bool searchByDepth = false;
	int searchDepth = 6;
	std::chrono::milliseconds searchTime = std::chrono::milliseconds(5000);
	std::chrono::steady_clock::time_point searchDeadline = std::chrono::steady_clock::now();

	bool useAlphaBetaPruning = true;
	bool useMoveSorting = true;

	bool useTranspositionTable = false;
};

extern EngineSettings engineSettings;

extern std::queue<std::string> msgQueue;
extern std::mutex msgQueueMutex;
extern std::condition_variable msgQueueReady;

extern json searchTimeGraphData;
extern json evalGraphData;

void SocketMessageHandler(std::string msg, Game& game);

bool UCIMessageHandler(std::string message, Game& game);

int EngineThread(const bool useUCI);
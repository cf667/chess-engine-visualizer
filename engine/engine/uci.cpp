#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <thread>
#include <sstream>

#include "uci.h"
#include "engine.h"

#pragma warning(push, 4)

void InitUCI()
{
	std::cout << "id name VEngine" << std::endl;
	std::cout << "id author cf667" << std::endl;
	std::cout << "uciok" << std::endl;

	std::string currentMessage;

	std::thread engineThread(EngineThread, true);
	engineThread.detach();

	while (true)
	{
		std::getline(std::cin, currentMessage);
		{
			std::lock_guard msgLock(msgQueueMutex);
			msgQueue.push(currentMessage);
		}
		msgQueueReady.notify_one();
	}
}

#pragma warning(pop)
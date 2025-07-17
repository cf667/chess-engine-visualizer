#define _CRT_SECURE_NO_WARNINGS

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <mutex>

#include "engine.h"
#include "game.h"
#include "util.h"
#include "evaluation.h"
#include "minimax.h"
#include "socket.h"

#pragma warning(push, 4)

std::queue<std::string> msgQueue;
std::mutex msgQueueMutex;
std::condition_variable msgQueueReady;

void MessageHandler(std::string msg, Game& game)
{
    json parsedMsg = json::parse(msg);
    switch (int(parsedMsg["id"]))
    {
    case 2:
    {
        std::string fen = parsedMsg["fen"];
        game = Game(fen.c_str());

        json message;
        message["id"] = 1;
        message["position"] = game.position;
        socketLoop->defer([message]() { mySocket->send(message.dump(), uWS::OpCode::TEXT); });
        break;
    }
    case 3:
    {
        char msgOrigin = uint8_t(parsedMsg["origin"]);
        char msgDestination = uint8_t(parsedMsg["destination"]);

        json message;
        message["id"] = 1;
        for (Move currentMove : game.GetLegalMoves())
        {
            if (currentMove.origin != msgOrigin || currentMove.destination != msgDestination) { continue; }
            game.MakeMove(currentMove);
            break;
        }
        message["position"] = game.position;
        socketLoop->defer([message]() { mySocket->send(message.dump(), uWS::OpCode::TEXT); });
        break;
    }
    case 4:
    {
        if (!IsRunning(GetGameState(game))) { break; }
        auto start = std::chrono::high_resolution_clock::now();
        Minimax(game, 3);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;
        std::cout << "totalDuration: " << duration << "\n";
        std::cout << "moveGen duration: " << totalDuration << "\n";
        game.MakeMove(game.bestMove);

        json message;
        message["id"] = 1;
        message["position"] = game.position;
        socketLoop->defer([message]() { mySocket->send(message.dump(), uWS::OpCode::TEXT); });
        break;
    }
    }

    return;
}

int EngineThread()
{
    Game game("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"); //starting position
    PrintPosition(game.position);
    std::cout << "Eval: " << EvaluatePosition(game) << std::endl;

    while (true)
    {
        std::unique_lock msgLock(msgQueueMutex);
        msgQueueReady.wait(msgLock, [] { return !msgQueue.empty(); }); //wait for websocket messages

        while (!msgQueue.empty())
        {
            const std::string curMsg = msgQueue.front();
            msgQueue.pop();
            msgLock.unlock();

            MessageHandler(curMsg, game);

            msgLock.lock();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

#pragma warning(pop)
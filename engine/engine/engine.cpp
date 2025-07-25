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

void SocketMessageHandler(std::string msg, Game& game)
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
        Minimax(game, 3, 0, true);
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

bool UCIMessageHandler(std::string message, Game& game)
{
    size_t temp;
    if ((temp = message.find("isready")) != std::string::npos)
    {
        std::cout << "readyok" << std::endl;
    }

    if ((temp = message.find("ucinewgame")) != std::string::npos)
    {

    }

    if ((temp = message.find("position")) != std::string::npos)
    {
        if (message.find("fen", temp + 8) != std::string::npos)
        {
            game = Game(message.substr(temp + 13, std::string::npos).c_str());
        }
        else if (message.find("startpos", temp + 8) != std::string::npos) { game = Game(); }

        if ((temp = message.find("moves", temp + 9)) != std::string::npos)
        {
            while ((temp = message.find(' ', temp + 1)) != std::string::npos)
            {
                std::vector<Move> possibleMoves = game.GetLegalMoves();
                char currentMoveOrigin = CoordToIndex(message.substr(temp + 1, temp + 3).c_str());
                char currentMoveDestination = CoordToIndex(message.substr(temp + 3, temp + 5).c_str());
                char currentMovePromotion = CharToPromotionType(message[temp + 5]);
                for (Move move : possibleMoves)
                {
                    if (currentMoveOrigin != move.origin) { continue; }
                    if (currentMoveDestination != move.destination) { continue; }
                    if (currentMovePromotion != QUIETMOVE && currentMovePromotion != GetPromotionType(move.flags)) { continue; }

                    game.MakeMove(move);
                    break;
                }
            }
        }
    }

    if ((temp = message.find("go")) != std::string::npos)
    {
        std::cout << "info score cp " << Minimax(game, 1) * 100 << std::endl;
        std::cout << "bestmove " << MoveToAlgebraic(game.bestMove) << std::endl;
    }

    if ((temp = message.find("quit")) != std::string::npos)
    {
        return false;
    }
    return true;
}

int EngineThread(const bool useUCI)
{
    Game game("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"); //starting position

    while (true)
    {
        std::unique_lock msgLock(msgQueueMutex);
        msgQueueReady.wait(msgLock, [] { return !msgQueue.empty(); }); //wait for websocket messages

        while (!msgQueue.empty())
        {
            const std::string curMsg = msgQueue.front();
            msgQueue.pop();
            msgLock.unlock();

            if (useUCI) 
            { 
                if (!UCIMessageHandler(curMsg, game)) { return 0; }
            }
            else { SocketMessageHandler(curMsg, game); }

            msgLock.lock();
        }
    }

    return 0;
}

#pragma warning(pop)
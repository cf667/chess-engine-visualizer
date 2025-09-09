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
#include "transposition_table.h"

#pragma warning(push, 4)

EngineSettings engineSettings;

std::queue<std::string> msgQueue;
std::mutex msgQueueMutex;
std::condition_variable msgQueueReady;

void SocketMessageHandler(std::string msg, Game& game)
{
    json parsedMsg = json::parse(msg);
    switch (int(parsedMsg["id"]))
    {
    case ID_FEN:
    {
        std::string fen = parsedMsg["fen"];
        game = Game(fen.c_str());

        json message;
        message["id"] = 1;
        message["position"] = game.position;
        socketLoop->defer([message]() { mySocket->send(message.dump(), uWS::OpCode::TEXT); });
        break;
    }
    case ID_MOVE:
    {
        char msgOrigin = uint8_t(parsedMsg["origin"]);
        char msgDestination = uint8_t(parsedMsg["destination"]);

        json message;
        message["id"] = 1;

        MoveList moveList = game.GetLegalMoves();
        for (int i = 0; i < moveList.count; i++)
        {
            if (moveList.list[i].origin != msgOrigin || moveList.list[i].destination != msgDestination) { continue; }
            game.MakeMove(moveList.list[i]);
            break;
        }

        message["position"] = game.position;
        socketLoop->defer([message]() { mySocket->send(message.dump(), uWS::OpCode::TEXT); });
        break;
    }
    case ID_MAKEBESTMOVE:
    {
        if (!IsRunning(GetGameState(game))) { break; }

        if (engineSettings.searchByDepth) { Minimax(game, engineSettings.searchDepth, -0x10000, 0x10000); }
        else { TimeSearch(game, engineSettings.searchTime); }
        
        game.MakeMove(game.bestMove);
        std::cout << MoveToAlgebraic(game.bestMove) << "\n";

        json message;
        message["id"] = 1;
        message["position"] = game.position;

        std::cout << "sending\n";
        socketLoop->defer([message]() { mySocket->send(message.dump(), uWS::OpCode::TEXT); });
        break;
    }
    case ID_SETTINGS:
    {
        engineSettings.connectedToGUI = true;
        engineSettings.searchByDepth = parsedMsg["searchByDepth"];
        engineSettings.searchDepth = parsedMsg["searchDepth"];
        engineSettings.searchTime = std::chrono::milliseconds(parsedMsg["searchTime"]);
        engineSettings.useAlphaBetaPruning = parsedMsg["useAlphaBetaPruning"];
        engineSettings.useMoveSorting = parsedMsg["useMoveSorting"];
        engineSettings.useTranspositionTable = parsedMsg["useTranspositionTable"];
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
        transpositionTable.clear();
    }

    if ((temp = message.find("position")) != std::string::npos)
    {
        transpositionTable.clear();

        if (message.find("fen", temp + 8) != std::string::npos)
        {
            game = Game(message.substr(temp + 13, std::string::npos).c_str());
        }
        else if (message.find("startpos", temp + 8) != std::string::npos) { game = Game(); }

        if ((temp = message.find("moves", temp + 9)) != std::string::npos)
        {
            while ((temp = message.find(' ', temp + 1)) != std::string::npos)
            {
                MoveList possibleMoves = game.GetLegalMoves();
                char currentMoveOrigin = CoordToIndex(message.substr(temp + 1, temp + 3).c_str());
                char currentMoveDestination = CoordToIndex(message.substr(temp + 3, temp + 5).c_str());
                char currentMovePromotion = CharToPromotionType(message[temp + 5]);
                Move move;
                for (int i = 0; i < possibleMoves.count; i++)
                {
                    move = possibleMoves.list[i];
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
        size_t endIndex;
        size_t startIndex;

        int msRemaining;
        int msIncrement;
        //std::cout << "info score cp " << Minimax(game, 3) * 100 << std::endl;
        if (game.toMove)
        {
            temp = message.find("wtime");
            startIndex = temp + 6;
            for (endIndex = 0; message[startIndex + endIndex] != ' '; endIndex++) {}
            msRemaining = std::stoi(message.substr(startIndex, endIndex));

            temp = message.find("winc");
            startIndex = temp + 5;
            for (endIndex = 0; message[startIndex + endIndex] != ' '; endIndex++) {}
            msIncrement = std::stoi(message.substr(startIndex, endIndex));
        }
        else
        {
            temp = message.find("btime");
            startIndex = temp + 6;
            for (endIndex = 0; message[startIndex + endIndex] != ' '; endIndex++) {}
            msRemaining = std::stoi(message.substr(startIndex, endIndex));

            temp = message.find("binc");
            startIndex = temp + 5;
            for (endIndex = 0; message[startIndex + endIndex] != ' '; endIndex++) {}
            msIncrement = std::stoi(message.substr(startIndex, endIndex));
        }

        std::chrono::milliseconds searchTime = std::chrono::milliseconds(msRemaining / 20 + msIncrement / 2);
        std::cout << "info score cp " << TimeSearch(game, searchTime) * 100 << std::endl;
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
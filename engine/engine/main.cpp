// IMPORT

#define _CRT_SECURE_NO_WARNINGS
#include <uwebsockets/App.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "game.h"
#include "util.h"

#pragma warning(push, 4)

std::queue<std::string> msgQueue;
std::mutex msgQueueMutex;
std::condition_variable msgQueueReady;

void MessageHandler(std::string msg, Game& game, uWS::WebSocket<false, true, void*>* ws, uWS::Loop* wsLoop)
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
        wsLoop->defer([ws, message]() { ws->send(message.dump(), uWS::OpCode::TEXT); });
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
        wsLoop->defer([ws, message]() { ws->send(message.dump(), uWS::OpCode::TEXT); });
        break;
    }
    }

    return;
}

int EngineThread(uWS::WebSocket<false, true, void*>* ws, uWS::Loop* wsLoop)
{
    Game game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "); //starting position

    while (true)
    {
        std::unique_lock msgLock(msgQueueMutex);
        msgQueueReady.wait(msgLock, [] { return !msgQueue.empty(); }); //wait for websocket messages

        while (!msgQueue.empty())
        {
            const std::string curMsg = msgQueue.front();
            msgQueue.pop();
            msgLock.unlock();

            MessageHandler(curMsg, game, ws, wsLoop);

            msgLock.lock();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

int main() 
{
    // INIT SOCKETS

    auto webSocket = uWS::App().ws<void*>
        (
            "/*",
            {
                .maxPayloadLength = 16 * 1024, //16KB
                .open = [](auto* ws)
                {
                    std::cout << "opened websocket connection" << std::endl;

                    std::thread engineThread(EngineThread, ws, uWS::Loop::get());
                    engineThread.detach();
                },
                .message = [](auto* ws, std::string_view message, uWS::OpCode opCode)
                {
                    ws; message; opCode;

                    //send message to EngineThread
                    {
                        std::lock_guard msgLock(msgQueueMutex);
                        msgQueue.push(std::string(message));
                    }
                    msgQueueReady.notify_one();
                },
                .drain = [](auto* ws)
                {
                    ws;
                    /* Check getBufferedAmount here */
                },
                .ping = [](auto* ws, std::string_view message)
                {
                    ws; message;
                },
                .pong = [](auto* ws, std::string_view message)
                {
                    ws; message;
                },
                .close = [](auto* ws, int code, std::string_view message)
                {
                    ws; code; message;
                    std::cout << "websocket connection closed" << std::endl;
                }
            }
        );

    webSocket.listen
    (
        8123, //port to listen on
        [](auto* ws)
            {
                if (ws) {
                    std::cout << "websocket listening on port 8123" << std::endl;
                }
                else {
                    std::cerr << "websocket listen failed";
                }
            }
    );
    
    webSocket.run();
    return 0;
}

#pragma warning(pop)
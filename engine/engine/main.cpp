// IMPORT

#define _CRT_SECURE_NO_WARNINGS
#include <uwebsockets/App.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>

#include "game.h"
#include "util.h"

#pragma warning(push, 4)

int main() 
{
    Game game("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    PrintPosition(game.position);
    /*std::cout << "toMove: " << int(game.toMove) << std::endl;
    std::cout << "castling: " << int(game.gameRules.castlingAbility) << std::endl;
    std::cout << "enPassant: " << int(game.gameRules.enPassantTarget) << std::endl;
    std::cout << "moves: " << int(game.gameRules.halfMoveCounter) << std::endl;*/

    // std::vector<Move> moveList = game.GetLegalMoves();

    /*std::cout << std::endl;
    for (int i = 0; i < moveList.size(); i++)
    {
        std::cout << int(moveList[i].origin) << " " << int(moveList[i].destination) << std::endl;
    }*/

    //debug if RevertMove() works properly

    /*printPosition(game.position);
    std::cout << std::endl;
    std::cout << "toMove: " << int(game.toMove) << std::endl;
    std::cout << "castling: " << int(game.gameRules.castlingAbility) << std::endl;
    std::cout << "enPassant: " << int(game.gameRules.enPassantTarget) << std::endl;
    std::cout << "moves: " << int(game.gameRules.halfMoveCounter) << std::endl;*/

    std::cout << "moegliche Zuege: " << std::endl;
    Perft(game, 4, 1);


    // INIT SOCKETS

    auto webSocket = uWS::App().ws<void*>
        (
            "/*",
            {
                .maxPayloadLength = 16 * 1024,
                .open = [&game](auto* ws)
                {
                    std::cout << "opened websocket connection" << std::endl;
                    json message;
                    message["id"] = 1;
                    message["position"] = game.position;
                    ws->send(message.dump(), uWS::OpCode::TEXT);
                    Log("log this", ws);
                },
                .message = [](auto* ws, std::string_view message, uWS::OpCode opCode)
                {
                    ws; message; opCode;
                    Log(std::string(message), ws);
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
        8123, 
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

    std::cout << "exiting..." << std::endl;
    return 0;
}

#pragma warning(pop)
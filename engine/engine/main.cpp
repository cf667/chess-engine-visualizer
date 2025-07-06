// IMPORT

#include <uwebsockets/App.h>
#include <nlohmann/json.hpp>
#include <iostream>

#include "game.h"

using json = nlohmann::json;

int main() 
{
    Game game("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    PrintPosition(game.position);
    std::cout << std::endl;
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

    std::cout << "\n\nmoegliche Zuege: \n" << Perft(game, 4) << std::endl;


    // INIT SOCKETS

    uWS::App().ws<void*>("/*", {
        .maxPayloadLength = 16 * 1024,
        .open = [](auto* ws) {
            std::cout << "established connection" << std::endl;
        },
        .message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
            std::cout << message << std::endl;
            ws->send(message, opCode);
        },
        .drain = [](auto* ws) {
            /* Check getBufferedAmount here */
        },
        .ping = [](auto* ws, std::string_view message) {

        },
        .pong = [](auto* ws, std::string_view message) {

        },
        .close = [](auto* ws, int code, std::string_view message) {
            std::cout << "closed connection" << std::endl;
        }
    }).listen(8123, [](auto* socket) {
        if (socket) {
            //std::cout << "listening on port 8123" << std::endl;
        }
        else {
            std::cerr << "error" << std::endl;
        }
    }).run();

    std::cout << "exiting..." << std::endl;
    return 0;
}
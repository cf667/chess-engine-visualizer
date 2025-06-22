// IMPORT

#include <uwebsockets/App.h>
#include <nlohmann/json.hpp>
#include <iostream>

#include "game.h"

using json = nlohmann::json;
int test[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

int main() {
    std::cout << test << std::endl;

    Game game("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2 ");
    printPosition(game.position);
    std::vector<Move> moveList = game.GetLegalMoves();
    for (int i = 0; i < moveList.size(); i++)
    {
        std::cout << std::endl << int(moveList[i].origin) << " " << int(moveList[i].destination);
    }
    printPosition(game.position);
    std::cout << std::endl;
    std::cout << "toMove: " << int(game.toMove) << std::endl;
    std::cout << "castling: " << int(game.gameRules.castlingAbility) << std::endl;
    std::cout << "enPassant: " << int(game.gameRules.enPassantTarget) << std::endl;
    std::cout << "moves: " << int(game.gameRules.halfMoveCounter) << std::endl;

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
            std::cout << "listening on port 8123" << std::endl;
        }
        else {
            std::cerr << "error" << std::endl;
        }
    }).run();

    std::cout << "exiting..." << std::endl;
    return 0;
}
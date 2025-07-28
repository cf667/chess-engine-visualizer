#define _CRT_SECURE_NO_WARNINGS

#include "socket.h"
#include "uci.h"
#include "minimax.h"
#include "util.h"

#pragma warning(push, 4)

int main()
{
    // INIT SOCKETS

    std::string startupMode;
    std::cin >> startupMode;

    if (startupMode == "uci") { InitUCI(); }
    else if (startupMode == "socket") { InitSocket(); }
    else if (startupMode == "test")
    {
        // testing speed
        int depth;
        std::cin >> depth;

        Game game = Game("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
        auto startTime = std::chrono::high_resolution_clock::now();

        std::cout << std::endl;

        for (int i = 1; i <= depth; i++)
        {
            auto startTime = std::chrono::high_resolution_clock::now();
            Minimax(game, i, -0x10000, 0x10000, 0, false);
            std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - startTime;
            std::cout << "Depth " << i << ": " << MoveToAlgebraic(game.bestMove) << " in " << duration << "\n";
        }
        std::cout << "Total nodes searched: " << totalNodesSearched << "\n";
    }

    return 0;
}

#pragma warning(pop)
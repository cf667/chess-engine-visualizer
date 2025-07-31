#define _CRT_SECURE_NO_WARNINGS

#include "socket.h"
#include "uci.h"
#include "minimax.h"
#include "iterative_deepening.h"
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
        Game game = Game("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

        /*Move move;
        move.Init(CoordToIndex("e1"), CoordToIndex("f1"), QUIETMOVE, EMPTY);
        game.MakeMove(move);

        move.Init(CoordToIndex("a8"), CoordToIndex("b8"), QUIETMOVE, EMPTY);
        game.MakeMove(move);

        move.Init(CoordToIndex("a1"), CoordToIndex("b1"), QUIETMOVE, EMPTY);
        game.MakeMove(move);

        move.Init(CoordToIndex("e8"), CoordToIndex("g8"), CASTLE_KING, EMPTY);
        game.MakeMove(move);

        PrintPosition(game.position);*/

        std::cin >> startupMode;
        if (startupMode == "depth")
        {
            int depth;
            std::cin >> depth;

            auto startTime = std::chrono::high_resolution_clock::now();

            std::cout << std::endl;

            for (int i = 1; i <= depth; i++)
            {
                auto startTime = std::chrono::high_resolution_clock::now();
                Minimax(game, i, -0x10000, 0x10000, 0, false, std::nullopt);
                std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - startTime;
                std::cout << "Depth " << i << ": " << MoveToAlgebraic(game.bestMove) << " in " << duration << "\n";
            }
            std::cout << "Total nodes searched: " << totalNodesSearched << "\n";
        }
        else if (startupMode == "time")
        {
            int time;
            std::cin >> time;

            int reachedDepth = 0;

            auto deadline = std::chrono::high_resolution_clock::now() + std::chrono::seconds(time);
            TimeSearch(game, deadline, &reachedDepth);
            std::cout << "Reached depth " << reachedDepth << " - best move: " << MoveToAlgebraic(game.bestMove) << "\n";
        }
        else if (startupMode == "perft")
        {
            int depth;
            std::cin >> depth;

            Perft(game, depth);
        }
    }

    return 0;
}

#pragma warning(pop)
#define _CRT_SECURE_NO_WARNINGS

#include "socket.h"
#include "uci.h"
#include "minimax.h"
#include "iterative_deepening.h"
#include "util.h"
#include "evaluation.h"
#include "transposition_table.h"
#include "engine.h"

#pragma warning(push, 4)

int main()
{
    engineSettings.searchDepth = 7;

    // INIT SOCKETS

    std::string startupMode;
    std::cin >> startupMode;

    if (startupMode == "uci") { InitUCI(); }
    else if (startupMode == "socket") { InitSocket(); }
    else if (startupMode == "test")
    {
        while (true)
        {
            Game game = Game("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ");

            std::cin >> startupMode;
            if (startupMode == "depth")
            {
                int depth;
                std::cin >> depth;

                std::cout << std::endl;

                int previousNodeCount = 0;

                for (int i = 1; i <= depth; i++)
                {
                    auto startTime = std::chrono::high_resolution_clock::now();
                    int score = Minimax(game, i, -0x10000, 0x10000);
                    std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - startTime;
                    //std::cout << "Depth " << i << ": " << MoveToAlgebraic(game.bestMove) << " in " << duration << " and " << totalNodesSearched - previousNodeCount << " nodes, score: " << score << "\n";
                    std::cout << "Depth " << i << " in " << duration << " and " << totalNodesSearched - previousNodeCount << " nodes" << "\n";
                    previousNodeCount = totalNodesSearched;
                }
                std::cout << "Total nodes searched: " << totalNodesSearched << "\n";
            }
            else if (startupMode == "time")
            {
                int time;
                std::cin >> time;

                int reachedDepth = 0;

                std::chrono::milliseconds searchTime = std::chrono::milliseconds(time * 1000);
                TimeSearch(game, searchTime, &reachedDepth);
                std::cout << "Reached depth " << reachedDepth << " - best move: " << MoveToAlgebraic(game.bestMove) << "\n";
            }
            else if (startupMode == "perft")
            {
                int depth;
                std::cin >> depth;
                auto startTime = std::chrono::high_resolution_clock::now();
                Perft(game, depth);
                std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - startTime;
                std::cout << "duration: " << duration;
            }

			transpositionTable.clear();
        }

        // testing speed
        

        /*Move move;
        move.Init(CoordToIndex("c3"), CoordToIndex("a4"), QUIETMOVE, EMPTY);
        game.MakeMove(move);

        move.Init(CoordToIndex("f6"), CoordToIndex("h5"), QUIETMOVE, EMPTY);
        game.MakeMove(move);

        move.Init(CoordToIndex("a4"), CoordToIndex("c3"), QUIETMOVE, EMPTY);
        game.MakeMove(move);

        move.Init(CoordToIndex("h5"), CoordToIndex("f6"), QUIETMOVE, EMPTY);
        game.MakeMove(move);

        move.Init(CoordToIndex("c3"), CoordToIndex("a4"), QUIETMOVE, EMPTY);
        game.MakeMove(move);

        move.Init(CoordToIndex("f6"), CoordToIndex("h5"), QUIETMOVE, EMPTY);
        game.MakeMove(move);

        move.Init(CoordToIndex("a4"), CoordToIndex("c3"), QUIETMOVE, EMPTY);
        game.MakeMove(move);

        move.Init(CoordToIndex("h5"), CoordToIndex("f6"), QUIETMOVE, EMPTY);
        game.MakeMove(move);

        std::cout << transpositionTable[game.hashKey].repetition << "\n";
		std::cout << GetGameStateValue(GetGameState(game)) << "\n";
        PrintPosition(game.position);*/

        
    }

    return 0;
}

#pragma warning(pop)
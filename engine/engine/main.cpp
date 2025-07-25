#define _CRT_SECURE_NO_WARNINGS

#include "socket.h"
#include "uci.h"
#include "minimax.h"

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

        Game game;
        auto startTime = std::chrono::high_resolution_clock::now();

        std::cout << std::endl;

        for (int i = 1; i <= depth; i++)
        {
            auto startTime = std::chrono::high_resolution_clock::now();
            Minimax(game, i);
            std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - startTime;
            std::cout << "Depth " << i << ": " << duration << "\n";
        }
    }

    return 0;
}

#pragma warning(pop)
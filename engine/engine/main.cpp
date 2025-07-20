#define _CRT_SECURE_NO_WARNINGS

#include "socket.h"
#include "uci.h"

#pragma warning(push, 4)

int main()
{
    // INIT SOCKETS

    std::string startupMode;
    std::cin >> startupMode;

    if (startupMode == "uci") { InitUCI(); }
    else { InitSocket(); }

    return 0;
}

#pragma warning(pop)
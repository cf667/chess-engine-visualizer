// IMPORT

#include <uwebsockets/App.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

int main() {
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
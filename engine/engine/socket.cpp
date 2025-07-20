#define _CRT_SECURE_NO_WARNINGS

#include "socket.h"
#include "engine.h"

#pragma warning(push, 4)

uWS::WebSocket<false, true, void*>* mySocket;
uWS::Loop* socketLoop;

void SendJSON(json message)
{
    socketLoop->defer([message]() { mySocket->send(message.dump(), uWS::OpCode::TEXT); });
    return;
}

void SendNewNode(int nodeId, int depth, int parentId, std::string previousMove)
{
    json message;
    message["id"] = 5;
    message["nodeId"] = nodeId;
    message["depth"] = depth;
    message["parentId"] = parentId;
    message["previousMove"] = previousMove;
    SendJSON(message);
    return;
}

void SendNodeScore(int nodeId, float score)
{
    json message;
    message["id"] = 6;
    message["nodeId"] = nodeId;
    message["score"] = score;
    SendJSON(message);
    return;
}

void InitSocket()
{
    auto app = uWS::App().ws<void*>
        (
            "/*",
            {
                .maxPayloadLength = 16 * 1024, //16KB
                .open = [](auto* ws)
                {
                    mySocket = ws;
                    socketLoop = uWS::Loop::get();

                    std::cout << "opened websocket connection" << std::endl;

                    std::thread engineThread(EngineThread, false);
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

    app.listen
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

    app.run();
}

#pragma warning(pop)
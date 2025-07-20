#pragma once
#include <uwebsockets/App.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

extern uWS::WebSocket<false, true, void*>* mySocket;
extern uWS::Loop* socketLoop;

void SendJSON(json message);

void SendNewNode(int nodeId, int depth, int parentId, std::string previousMove);

void SendNodeScore(int nodeId, float score);

void InitSocket();
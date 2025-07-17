#pragma once
#include <uwebsockets/App.h>

extern uWS::WebSocket<false, true, void*>* mySocket;
extern uWS::Loop* socketLoop;

void InitSocket();
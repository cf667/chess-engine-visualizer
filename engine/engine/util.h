#pragma once
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <uwebsockets/App.h>

void Log(std::string str, uWS::WebSocket<false, true, void*>* ws);
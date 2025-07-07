#define _CRT_SECURE_NO_WARNINGS
#include "util.h"

#pragma warning(push, 4)

void Log(std::string str, uWS::WebSocket<false, true, void*>* ws)
{
	std::cout << str << "\n";
	if (ws)
	{
		json log;
		log["id"] = 0;
		log["log"] = "ENGINE: " + str;
		ws->send(log.dump(), uWS::OpCode::TEXT);
	}
	return;
}

#pragma warning(pop)
#include "util.h"

void Log(std::string str, uWS::WebSocket<false, true, void*>* ws)
{
	std::cout << str << "\n";
	if (ws)
	{
		json log;
		log["id"] = 0;
		log["log"] = str;
		ws->send(log.dump(), uWS::OpCode::TEXT);
	}
	return;
}
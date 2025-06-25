///************************* OUTS HOMEWORK ****************************************

#include "cEndPoint.hpp"
#include "cMessage.hpp"
#include "nlohmann/json.hpp"

std::shared_ptr<cInterpretCommand> cEndPoint::parse(const cMessage& msg)
{
  cMsgHeader h;

  nlohmann::json j = nlohmann::json::parse(msg.Header());
  from_json(j, h);

  //ioc.Resolve("Game",h.gameId.id);
  //ioc.Resolve("Object", h.gameId.id);
}

///************************* OUTS HOMEWORK ****************************************

#include "cEndPoint.hpp"
#include "cMessage.hpp"
#include "nlohmann/json.hpp"
#include "cIoC.hpp"
#include "cGame.hpp"
#include "cJsonString.hpp"
#include "cInterpretCommand.hpp"
#include "cException.hpp"

void cEndPoint::Register(cGame* game)
{
	games[game->Id()] = game;
}

void cEndPoint::process(const cMessage& msg)
{
  // get game 
  cMsgHeader h;
  nlohmann::json j = nlohmann::json::parse(msg.Header());
  from_json(j, h);

  // create command
  cInterpretCommand *pcmd =  ioc->Resolve<cInterpretCommand>("A", "cInterpretCommand", msg);
  std::shared_ptr<iCommand> cmd(pcmd);
  
  // push command to game's command deque
  cGame* game = games[h.gameId.id];
  if (game == nullptr)
    throw(cException("there is not such registered game"));
  else
    game->push_back(cmd);
}

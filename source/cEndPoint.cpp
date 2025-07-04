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

#include <gtest/gtest.h>

void cEndPoint::process(const cMessage& msg)
{
  // get game 
  cMsgHeader h;
  nlohmann::json j = nlohmann::json::parse(msg.Header());
  from_json(j, h);

  sInterpretCommandData sd;
  // create command
  // push command to game's command deque
  sd.game = games[h.gameId.id];
  EXPECT_TRUE(sd.game != nullptr);
  sd.msg = &msg;

  cInterpretCommand *pcmd =  ioc->Resolve<cInterpretCommand>("A", "cInterpretCommand", sd );
  EXPECT_TRUE(pcmd != nullptr);
  std::shared_ptr<iCommand> cmd(pcmd);
  
  if (sd.game == nullptr)
    throw(cException("there is not such registered game"));
  else
    sd.game->push_back(cmd);
}

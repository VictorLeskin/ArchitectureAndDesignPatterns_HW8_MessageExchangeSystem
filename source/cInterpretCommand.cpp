///************************* OUTS HOMEWORK ****************************************

#include "cInterpretCommand.hpp"
#include "cMessage.hpp"
#include "cGame.hpp"

void cInterpretCommand::Execute()
{
  game->Execute(*this);
}
  ///************************* OUTS HOMEWORK ****************************************

#include <gtest/gtest.h>

#include "ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem.hpp"
#include "cMessage.hpp"
#include "cEndPoint.hpp"
#include "test_aMessageBroker.h"

#include <iostream>
#include <nlohmann/json.hpp>

// gTest grouping class
class test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem : public ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem
  {
  public:
    // add here members for free access.
    using ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem::ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem; // delegate constructors
  };

};
 
TEST_F(test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem, test_ctor )
{
  Test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem t;
}

class iCommand
{
public:
};

class cGame
{
public:
  void push_back(std::shared_ptr<iCommand>&);
};

class cObject
{
public:

};

class cInterpretCommand
{
public:
  cGame* Game() const
  {
    throw(std::exception("Not implemented"));
    return nullptr;
  }
  std::shared_ptr<iCommand>& Command() const
  {
    throw(std::exception("Not implemented"));
    return std::shared_ptr<iCommand>();
  }
};

class cIoC
{
public:
  cObject* Resolve(const char* sz, const std::string& id) { return nullptr; }
  cObject* Resolve(const char* sz, const std::string& gameId, const std::string& id) { return nullptr; }
};

TEST_F(test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem, test_EndpointCommonBehaviour )
{
  // create message broker.
  test_aMessageBroker::Test_aMessageBroker broker;
  cIoC IoC;
  cEndPoint endPoint;

  endPoint.set(IoC);


  cMessage m;
  while (true == broker.get(m))
  {
    std::shared_ptr<cInterpretCommand> cmd = endPoint.parse(m);
    cmd->Game()->push_back(cmd->Command());
  }
}

void cGame::push_back(std::shared_ptr<iCommand>&)
{
  throw(std::exception("Not implemented"));
}

  ///************************* OUTS HOMEWORK ****************************************

#include <gtest/gtest.h>

#include "ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem.hpp"
#include "cMessage.hpp"
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

class cObject
{

};

class cEndPoint
{
public:

  std::unique_ptr<cGameOperation> Parse(const cMessage m) { return nullptr;  }
};

class cIoC
{
public:
  cObject* Resolve(const char* sz, const std::string & id) { return nullptr; }
  cObject* Resolve(const char* sz, const std::string& gameId, const std::string& id) { return nullptr; }

};
TEST_F(test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem, test_EndpointCommonBehaviour )
{
  // create message broker.
  test_aMessageBroker::Test_aMessageBroker broker;
  cEndPoint endPoint;
  cIoC IoC;

  cMessage m;
  while (true == broker.get(m))
  {
    std::unique_ptr<cGameOperation> cmd = endPoint.Parse(m);

    cObject *game = IoC.Resolve("Game", cmd->gameId.id );
    cObject *obj  = IoC.Resolve("Object", cmd->gameId.id, cmd->objId.id);
  }
}
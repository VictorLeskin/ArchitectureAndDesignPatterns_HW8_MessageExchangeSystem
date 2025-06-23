///************************* OUTS HOMEWORK ****************************************

#include <gtest/gtest.h>

#include "cMessage.hpp"

// gTest grouping class
class test_cMessage : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_cMessage : public cMessage
  {
  public:
    // add here members for free access.
    Test_cMessage(const cMessage& m) : cMessage(m) {}
    using cMessage::cMessage; // delegate constructors
  };

};



TEST_F(test_cMessage, test_Create)
{
  cGameId gameId{ "gameId" };
  cOjectId ojectId{ "ojectId" };
  cOperationId operationId{ "operationId" };

  cOperationParameters opParameter{ 22, cVector(303,404) };

  Test_cMessage t = cMessage::Create(gameId, ojectId, operationId, opParameter );
  std::string res = R""""({"gameId":"gameId","objId":"ojectId","operationId":"operationId","operationParmeters":{"pos":{"x":303.0,"y":404.0},"type":22}})"""";
  EXPECT_EQ(res, t.str() );
}


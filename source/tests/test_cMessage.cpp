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

TEST_F(test_cMessage, test_to_msg )
{
  TGameOperation<cOperationParameters> k;
  k.gameId = cGameId{ "gameId" };
  k.objId = cOjectId{ "ojectId" };
  k.operationId = cOperationId{ "operationId" };
  k.operationParameters = cOperationParameters{ 22, cVector(303,404) };

  Test_cMessage t("");
  cMessage::to_msg( t, k );
  std::string res = R""""({"gameId":"gameId","objId":"ojectId","operationId":"operationId","operationParmeters":{"pos":{"x":303.0,"y":404.0},"type":22}})"""";
  EXPECT_EQ(res, t.str());
}

TEST_F(test_cMessage, test_from_msg)
{
  std::string res = R""""({"gameId":"gameId","objId":"ojectId","operationId":"operationId","operationParmeters":{"pos":{"x":303.0,"y":404.0},"type":22}})"""";
  cMessage t(res);

  TGameOperation<cOperationParameters> k;
  cMessage::from_msg(t, k);

  EXPECT_EQ(std::string("gameId"), k.gameId.id);
  EXPECT_EQ(std::string("ojectId"), k.objId.id);
  EXPECT_EQ(std::string("operationId"), k.operationId.id);
  EXPECT_EQ(22, k.operationParameters.type);
  EXPECT_EQ(303, k.operationParameters.pos.x );
  EXPECT_EQ(404, k.operationParameters.pos.y);
}
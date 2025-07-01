///************************* OUTS HOMEWORK ****************************************

#include <gtest/gtest.h>

#include "cEndPoint.hpp"
#include "cMessage.hpp"

// gTest grouping class
class test_cEndPoint : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_cEndPoint : public cEndPoint
  {
  public:
    // add here members for free access.
    using cEndPoint::cEndPoint; // delegate constructors
    using cEndPoint::process;
  };

};
 
TEST_F(test_cEndPoint, test_ctor )
{
  Test_cEndPoint t;
}

TEST_F(test_cEndPoint, test_process)
{
  {
    Test_cEndPoint t;
    const char* szHeader     = R""""({"gameId":"Game #1","objId":"SpaceShip #1","operationId":"moveTo"})"""";
    const char* szParameters = R""""({"x":23.0,"y":45.0})"""";
    cMessage msg(szHeader, szParameters);

    t.process(msg);
  }
}





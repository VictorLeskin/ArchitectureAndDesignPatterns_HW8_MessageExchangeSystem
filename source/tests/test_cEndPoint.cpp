///************************* OUTS HOMEWORK ****************************************

#include <gtest/gtest.h>

#include "cEndPoint.hpp"

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
  };

};
 
TEST_F(test_cEndPoint, test_ctor )
{
  Test_cEndPoint t;
}


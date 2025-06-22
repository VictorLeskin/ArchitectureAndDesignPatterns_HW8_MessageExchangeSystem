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
    using cMessage::cMessage; // delegate constructors
  };

};
 
TEST_F(test_cMessage, test_ctor )
{
  Test_cMessage t;
}


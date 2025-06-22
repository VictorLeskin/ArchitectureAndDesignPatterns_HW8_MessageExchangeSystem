///************************* OUTS HOMEWORK ****************************************

#include <gtest/gtest.h>

#include "aMessageBroker.hpp"
#include "cMessage.hpp"
#include <optional>

// gTest grouping class
class test_aMessageBroker : public ::testing::Test
{
public:
  // additional class to access to member of tested class
  class Test_aMessageBroker : public aMessageBroker
  {

  public:
      std::optional<cMessage> msg;

      bool get(cMessage& m) override
      {
          if (msg.has_value())
          {
              m = msg.value();
              msg.reset();
              return true;
          }
          else
              return false;
                

      }
  public:
    // add here members for free access.
    using aMessageBroker::aMessageBroker; // delegate constructors
  };

};
 
TEST_F(test_aMessageBroker, test_ctor )
{
  Test_aMessageBroker t;
}


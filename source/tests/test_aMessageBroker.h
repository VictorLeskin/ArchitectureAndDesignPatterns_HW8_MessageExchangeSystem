#ifndef TEST_AMESSAGEBROKER_H
#define TEST_AMESSAGEBROKER_H

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

    void put(cMessage& m)
    {
      msg = m;
    }
  public:
    // add here members for free access.
    using aMessageBroker::aMessageBroker; // delegate constructors
  };

};
#endif //#ifndef TEST_AMESSAGEBROKER_H

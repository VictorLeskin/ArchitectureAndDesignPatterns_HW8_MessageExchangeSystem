///************************* OUTS HOMEWORK ****************************************
#ifndef CENDPOINT_HPP
#define CENDPOINT_HPP

#include <memory>
 
class cInterpretCommand;
class cMessage;
class cIoC;

class cEndPoint
{
  public:
    std::shared_ptr<cInterpretCommand> parse(const cMessage& m);
  void set(cIoC& IoC) { ioc = &IoC;  }

protected:
  cIoC* ioc;
};

#endif //#ifndef CENDPOINT_HPP

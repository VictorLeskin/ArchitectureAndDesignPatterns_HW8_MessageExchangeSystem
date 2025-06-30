///************************* OUTS HOMEWORK ****************************************
#ifndef CINTERPRETCOMMAND_HPP
#define CINTERPRETCOMMAND_HPP

#include "iCommand.hpp"
#include "cException.hpp"
#include <memory>

class cOperationData;

class cInterpretCommand : public iCommand
{
public:
	cInterpretCommand(const std::string &objId, const std::string &operationId, const std::shared_ptr<cOperationData> &operationData );

	virtual void Execute() { throw(cException("not implemented")); }
	virtual const char* Type() { throw(cException("not implemented")); }

};


#endif //#ifndef CINTERPRETCOMMAND_HPP

///************************* OUTS HOMEWORK ****************************************
#ifndef CINTERPRETCOMMAND_HPP
#define CINTERPRETCOMMAND_HPP

#include "iCommand.hpp"
#include "cException.hpp"
#include <memory>

class cInterpretCommand : public iCommand
{
public:
	std::shared_ptr<iCommand> Command() const
	{
		throw(cException("Not implemented"));
		return std::shared_ptr<iCommand>();
	}
};


#endif //#ifndef CINTERPRETCOMMAND_HPP

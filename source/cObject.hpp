///************************* OUTS HOMEWORK ****************************************
#ifndef COBJECT_HPP
#define COBJECT_HPP

#include <string>

class cObject
{
public:
	cObject(std::string id) : id(id) {}

	virtual ~cObject() {}

	const std::string& Id() const { return id; }

protected:
	std::string id;
};

#endif //#ifndef COBJECT_HPP

///************************* OUTS HOMEWORK ****************************************
#ifndef CSPACESHIP_HPP
#define CSPACESHIP_HPP

#include "cObject.hpp"
#include "cVector.hpp"

class cSpaceShip : public cObject
{
public:
	cSpaceShip(std::string id) : cObject(id), p({0,0}) {}

	void Execute(const cInterpretCommand& cmd) override;

	void moveTo(const cVector& newPos) { p = newPos;  }
	const cVector& Position() const { return p; }

protected:
	cVector p;
};


#endif //#ifndef CSPACESHIP_HPP

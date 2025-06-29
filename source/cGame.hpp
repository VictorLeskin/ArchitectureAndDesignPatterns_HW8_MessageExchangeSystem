///************************* OUTS HOMEWORK ****************************************
#ifndef CGAME_HPP
#define CGAME_HPP

#include "cObject.hpp"
#include "cGameId.hpp"

#include <memory>

class cSpaceShip;
class iCommand;

class cGame : public cObject
{
public:
	cGame(std::string id) : cObject(id) {}

	void Register(cSpaceShip* spaceShip1) {}

	void push_back(std::shared_ptr<iCommand>&);
};

#endif //#ifndef CGAME_HPP

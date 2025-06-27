///************************* OUTS HOMEWORK ****************************************

#include <gtest/gtest.h>

#include "ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem.hpp"
#include "cMessage.hpp"
#include "cEndPoint.hpp"
#include "cIoC.hpp"
#include "test_aMessageBroker.h"

#include <iostream>
#include <nlohmann/json.hpp>

// gTest grouping class
class test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem : public ::testing::Test
{
public:
	// additional class to access to member of tested class
	class Test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem : public ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem
	{
	public:
		// add here members for free access.
		using ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem::ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem; // delegate constructors
	};

	// additional class to access to member of tested class
	class Test_cFactory;


};

TEST_F(test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem, test_ctor)
{
	Test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem t;
}

class cObject
{
public:
	cObject(std::string id) : id(id) {}

	virtual ~cObject() {}

	const std::string& Id() const { return id; }

protected:
	std::string id;
};


class cSpaceShip : public cObject
{
public:
	cSpaceShip(std::string id) : cObject(id) {}
};

class cGame : public cObject
{
public:
	cGame(std::string id) : cObject(id) {}

	void Register(cSpaceShip* spaceShip1) {}

	void push_back(std::shared_ptr<iCommand>&);
};

class cJsonString;

// additional class to access to member of tested class
class test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem::Test_cFactory : public cFactory
{
public:
	static cGame* createGame( std::string id ) { return new cGame(id); }
	static cSpaceShip* createSpaceShip(std::string id) { return new cSpaceShip(id); }
	static cSpaceShip* createInterpretCommand( std::string gameId, std::string objectId, std::string operation, const cJsonString& operationParameters) 
	{ 
		return nullptr;
	}
};

class cInterpretCommand : public iCommand
{
public:
	cGame* Game() const
	{
		throw(cException("Not implemented"));
		return nullptr;
	}
	std::shared_ptr<iCommand> Command() const
	{
		throw(cException("Not implemented"));
		return std::shared_ptr<iCommand>();
	}
};


TEST_F(test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem, test_EndpointCommonBehaviour)
{
	// create message broker.
	test_aMessageBroker::Test_aMessageBroker broker;
	cIoC IoC;
	cEndPoint endPoint;

	Test_cFactory f1;

	f1.Register("cGame", Test_cFactory::createGame );
	f1.Register("cSpaceShip", Test_cFactory::createSpaceShip);

	// registering 
	const cFactory& f11 = f1;

	// register factory ( only one scope )
	IoC.Resolve<iCommand>("Register", "A", f11)->Execute();

	// register two factory methods for game and spaceship
	IoC.Resolve<iCommand>("Register", "A", "cGame", Test_cFactory::createGame )->Execute();
	IoC.Resolve<iCommand>("Register", "A", "cSpaceShip", Test_cFactory::createSpaceShip)->Execute();
	IoC.Resolve<iCommand>("Register", "A", "cInterpretCommand", Test_cFactory::createInterpretCommand)->Execute();
	


	cGame* game1 = IoC.Resolve<cGame>("A", "cGame", std::string("Game #1"));
	cGame* game2 = IoC.Resolve<cGame>("A", "cGame", std::string("Game #2"));

	cSpaceShip* spaceShip1 = IoC.Resolve<cSpaceShip>("A", "cSpaceShip", std::string("SpaceShip #1 "));
	cSpaceShip* spaceShip2 = IoC.Resolve<cSpaceShip>("A", "cSpaceShip", std::string("SpaceShip #2 "));
	cSpaceShip* spaceShip3 = IoC.Resolve<cSpaceShip>("A", "cSpaceShip", std::string("SpaceShip #3 "));
	cSpaceShip* spaceShip4 = IoC.Resolve<cSpaceShip>("A", "cSpaceShip", std::string("SpaceShip #4 "));

	game1->Register(spaceShip1);
	game1->Register(spaceShip2);
	game2->Register(spaceShip3);
	game2->Register(spaceShip4);

	cMessage m;
	while (true == broker.get(m))
	{
		std::shared_ptr<cInterpretCommand> cmd = endPoint.parse(m);
		cmd->Execute();
	}
}

void cGame::push_back(std::shared_ptr<iCommand>&)
{
	throw(cException("Not implemented"));
}

///************************* OUTS HOMEWORK ****************************************

#include <gtest/gtest.h>

#include "ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem.hpp"
#include "cMessage.hpp"
#include "cEndPoint.hpp"
#include "cIoC.hpp"
#include "cMessagesDeque.hpp"
#include "cGame.hpp"
#include "cInterpretCommand.hpp"

#include <iostream>
#include <nlohmann/json.hpp>
#include <tuple>
#include <memory>

#include "test_aMessageBroker.h"

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



class cSpaceShip : public cObject
{
public:
	cSpaceShip(std::string id) : cObject(id) {}
};


class cJsonString;
class cOperationData 
{
public:
	virtual void from_json(const char* sz) = 0;
};

template<const char *type, const char *action>
class TAction : public cOperationData
{
public:
	void from_json(const char* sz) override {}
};

template<const char* type, const char* command, typename DATA>
class TOperation : public cOperationData
{
public:
	void from_json(const char* sz) override
	{
		::from_json(sz,t);
	};
	DATA t;
};

extern const char str_SpaceShip[] = "SpaceShip";
extern const char str_Game[] = "Game";
extern const char str_moveTo[] = "moveTo";
extern const char str_stop[] = "stop";
extern const char str_pause[] = "pause";

// additional class to access to member of tested class
class test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem::Test_cFactory : public cFactory
{
public:
	static cGame* createGame( std::string id ) { return new cGame(id); }
	static cSpaceShip* createSpaceShip(std::string id) { return new cSpaceShip(id); }
	static cOperationData* createSerializeObj(const std::string& objId, const std::string& operationId)
	{
		// change to map
		// change to map
		if (objId.starts_with(str_SpaceShip) && operationId == str_moveTo)
			return new TOperation< str_SpaceShip, str_moveTo, cVector>;
		if (objId.starts_with(str_SpaceShip) && operationId == str_stop)
			return new TAction<str_SpaceShip, str_stop>;
		if (objId.starts_with(str_Game) && operationId == str_pause)
			return new TAction<str_Game, str_pause>;
	}

	static cVector* createInterpretCommand_moveTo() { return new cVector; }
	static cInterpretCommand* createInterpretCommand(const cMessage& msg)
	{ 
		// get game 
		cMsgHeader h;
		std::istringstream strm(msg.str());
		nlohmann::json j = nlohmann::json::parse(strm);
		from_json(j, h);

		cOperationData *p = createSerializeObj(h.objId.id, h.operationId.id);
		p->from_json(msg.str().c_str() + strm.tellg());

		return new cInterpretCommand(h.objId.id, h.operationId.id, std::shared_ptr<cOperationData>(p) );
	}


	static cVector* createInterpretCommandParameters() { return new cVector; }
};


TEST_F(test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem, test_sendMessage)
{
	using operation_parameters = std::tuple< int, cVector >;
	operation_parameters p{321,cVector(456,78)};
	TGameOperation<operation_parameters> op;

	op.gameId.id = "Game #1";
	op.objId.id = "Spaceship #1";
	op.operationId.id = "moveTo";
	op.operationParameters = p;

	cMessage msg = cMessage::Create(op);
	cMessagesDeque deq;

	deq.push_back(msg);

	EXPECT_EQ(1, deq.size());

	cMessage msg1;
	EXPECT_TRUE( deq.pop_front(msg1) );
	EXPECT_EQ(0, deq.size());
}

TEST_F(test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem, test_EndpointCommonBehaviour)
{
	// create message broker.
	test_aMessageBroker::Test_aMessageBroker broker;
	cIoC IoC;
	cEndPoint endPoint;

	Test_cFactory f1;
	const cFactory& f11 = f1;

	// registering 

	// register factory ( only one scope )
	IoC.Resolve<iCommand>("Register", "A", f11)->Execute();

	// register two factory methods for game and spaceship
	IoC.Resolve<iCommand>("Register", "A", "cGame", Test_cFactory::createGame )->Execute();
	IoC.Resolve<iCommand>("Register", "A", "cSpaceShip", Test_cFactory::createSpaceShip)->Execute();
	IoC.Resolve<iCommand>("Register", "A", "cInterpretCommand", Test_cFactory::createInterpretCommand)->Execute();

	// create games 
	cGame* game1 = IoC.Resolve<cGame>("A", "cGame", std::string("Game #1"));
	cGame* game2 = IoC.Resolve<cGame>("A", "cGame", std::string("Game #2"));

	endPoint.Register(game1);
	endPoint.Register(game2);

	cSpaceShip* spaceShip1 = IoC.Resolve<cSpaceShip>("A", "cSpaceShip", std::string("SpaceShip #1"));
	cSpaceShip* spaceShip2 = IoC.Resolve<cSpaceShip>("A", "cSpaceShip", std::string("SpaceShip #2"));
	cSpaceShip* spaceShip3 = IoC.Resolve<cSpaceShip>("A", "cSpaceShip", std::string("SpaceShip #3"));
	cSpaceShip* spaceShip4 = IoC.Resolve<cSpaceShip>("A", "cSpaceShip", std::string("SpaceShip #4"));

	game1->Register(spaceShip1);
	game1->Register(spaceShip2);
	game2->Register(spaceShip3);
	game2->Register(spaceShip4);

	cMessage m;
	while (true == broker.get(m))
		endPoint.process(m);

}

void cGame::push_back(std::shared_ptr<iCommand>&)
{
	throw(cException("Not implemented"));
}

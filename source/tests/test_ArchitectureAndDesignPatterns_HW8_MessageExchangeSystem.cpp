///************************* OUTS HOMEWORK ****************************************

#include <gtest/gtest.h>
#include "fstream"

#include "ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem.hpp"
#include "cMessage.hpp"
#include "cEndPoint.hpp"
#include "cIoC.hpp"
#include "cMessagesDeque.hpp"
#include "cGame.hpp"
#include "cInterpretCommand.hpp"
#include "cSpaceShip.hpp"
#include "cStopCommand.hpp"

#include <iostream>
#include <nlohmann/json.hpp>
#include <tuple>
#include <memory>
#include <thread>

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

	class Test_cIoC : public cIoC
	{
	public:
		using cIoC::factories;
	};
};

//TEST_F(test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem, test_ctor)
//{
//	//Test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem t;
//}

class cJsonString;

extern const char str_SpaceShip[] = "SpaceShip";
extern const char str_Game[] = "Game";
extern const char str_moveTo[] = "moveTo";
extern const char str_refuel[] = "refuel";

// additional class to access to member of tested class
class test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem::Test_cFactory : public cFactory
{
public:
	static cGame* createGame( std::string id ) { return new cGame(id); }
	static cSpaceShip* createSpaceShip(std::string id) { return new cSpaceShip(id); }
	static cOperationData* createSerializeObj(const std::string& objId, const std::string& operationId)
	{
		// change to map
		if (objId.starts_with(str_SpaceShip) && operationId == str_moveTo)
			return new TOperation< str_SpaceShip, str_moveTo, cVector>;
		if (objId.starts_with(str_SpaceShip) && operationId == str_refuel)
			return new TOperation<str_SpaceShip, str_refuel, cTanker>;

		return nullptr;
	}

	static cInterpretCommand* createInterpretCommand(const sInterpretCommandData &sd)
	{ 
		EXPECT_TRUE(nullptr == sd.game);
		EXPECT_TRUE(nullptr == sd.msg);

		if (sd.game == nullptr && sd.msg == nullptr)
			return nullptr;

		return nullptr;
		cGame* game = sd.game;
		const cMessage& msg = *sd.msg;

		// get game 
		cMsgHeader h;
		std::istringstream strm(msg.Header());
		nlohmann::json j = nlohmann::json::parse(strm);
		from_json(j, h);

		cOperationData *p = createSerializeObj(h.objId.id, h.operationId.id);
		p->from_json(msg.Parameters().c_str());

		return new cInterpretCommand(game, h.objId.id, h.operationId.id, std::shared_ptr<cOperationData>(p) );
	}
};


//TEST_F(test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem, test_sendMessage)
//{
//	using operation_parameters = std::tuple< int, cVector >;
//	operation_parameters p{321,cVector(456,78)};
//	TGameOperation<operation_parameters> op;
//
//	op.gameId.id = "Game #1";
//	op.objId.id = "Spaceship #1";
//	op.operationId.id = "moveTo";
//	op.operationParameters = p;
//
//	cMessage msg = cMessage::Create(op);
//	cMessagesDeque deq;
//
//	deq.push_back(msg);
//
//	EXPECT_EQ(1, deq.size());
//
//	cMessage msg1;
//	EXPECT_TRUE( deq.pop_front(msg1) );
//	EXPECT_EQ(0, deq.size());
//}


void DBG(int t)
{
	static int iEntry = 0;

	if (iEntry == 0)
		EXPECT_EQ(t, 0);
	if (iEntry == 1)
		EXPECT_EQ(t, 1);

	iEntry++;
}

void* qq;

void DBG1(void* method)
{
	EXPECT_EQ(qq, method );
}
TEST_F(test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem, test_0 )
{
	// create message broker.
	test_aMessageBroker::Test_aMessageBroker broker;
	Test_cIoC IoC;
	cEndPoint endPoint;

	Test_cFactory f1;

	f1.Register( "cGame", Test_cFactory::createGame);
	f1.Register( "cSpaceShip", Test_cFactory::createSpaceShip);
	f1.Register("cInterpretCommand", Test_cFactory::createInterpretCommand);

	qq = Test_cFactory::createInterpretCommand;

	const cFactory& f11 = f1;

	// register factory ( only one scope )
	IoC.Resolve<iCommand>("Register", "A", f11)->Execute();

	sInterpretCommandData sd;
	sd.game = nullptr;
	sd.msg = nullptr;

	EXPECT_EQ(1,IoC.factories.size());
	EXPECT_TRUE(IoC.factories.find("A") != IoC.factories.end());
	std::shared_ptr<cFactory> &f = IoC.factories["A"];

	EXPECT_EQ(3, f->factoryMethods.size());
	
	auto it = f->factoryMethods.begin(); 
	
	EXPECT_EQ( "cGame", it->first);
	EXPECT_EQ(Test_cFactory::createGame, it->second );
	++it;
	EXPECT_EQ("cInterpretCommand", it->first);
	EXPECT_EQ(Test_cFactory::createInterpretCommand, it->second);
	++it;
	EXPECT_EQ("cSpaceShip", it->first);
	EXPECT_EQ(Test_cFactory::createSpaceShip, it->second);

	sInterpretCommandData& sd1 = sd;
	cInterpretCommand* pcmd = IoC.Resolve<cInterpretCommand>("A", "cInterpretCommand", sd1);
	return;

	// create games 
	cGame* game1 = IoC.Resolve<cGame>("A", "cGame", std::string("Game #1"));
	
	cSpaceShip* spaceShip1 = IoC.Resolve<cSpaceShip>("A", "cSpaceShip", std::string("SpaceShip #1"));
	
	game1->Register(spaceShip1);
	
	endPoint.Register(game1);
	endPoint.set(IoC);

	// load two command to different games
	// moving direction for the first ship of the first game
	TGameOperation<cVector> moveTo;
	moveTo.gameId.id = "Game #1";
	moveTo.objId.id = "SpaceShip #1";
	moveTo.operationId.id = "moveTo";
	moveTo.operationParameters = cVector(23, 45);

	cMessage m1 = cMessage::Create(moveTo);
	broker.put(m1);

	cMessage m;
	while (true == broker.get(m))
	{
		sInterpretCommandData sd;
		sd.game = game1;
		sd.msg = &m;
		cInterpretCommand* pcmd = IoC.Resolve<cInterpretCommand>("A", "cInterpretCommand", sd);
	}
}



//TEST_F(test_ArchitectureAndDesignPatterns_HW8_MessageExchangeSystem, test_EndpointCommonBehaviour)
//{
//	// create message broker.
//	test_aMessageBroker::Test_aMessageBroker broker;
//	cIoC IoC;
//	cEndPoint endPoint;
//
//	Test_cFactory f1;
//	const cFactory& f11 = f1;
//
//	// registering 
//
//	// register factory ( only one scope )
//	IoC.Resolve<iCommand>("Register", "A", f11)->Execute();
//
//	// register two factory methods for game and spaceship
//	IoC.Resolve<iCommand>("Register", "A", "cGame", Test_cFactory::createGame )->Execute();
//	IoC.Resolve<iCommand>("Register", "A", "cSpaceShip", Test_cFactory::createSpaceShip)->Execute();
//	IoC.Resolve<iCommand>("Register", "A", "cInterpretCommand", Test_cFactory::createInterpretCommand)->Execute();
//
//	// create games 
//	cGame* game1 = IoC.Resolve<cGame>("A", "cGame", std::string("Game #1"));
//	cGame* game2 = IoC.Resolve<cGame>("A", "cGame", std::string("Game #2"));
//
//	cSpaceShip* spaceShip1 = IoC.Resolve<cSpaceShip>("A", "cSpaceShip", std::string("SpaceShip #1"));
//	cSpaceShip* spaceShip2 = IoC.Resolve<cSpaceShip>("A", "cSpaceShip", std::string("SpaceShip #2"));
//	cSpaceShip* spaceShip3 = IoC.Resolve<cSpaceShip>("A", "cSpaceShip", std::string("SpaceShip #3"));
//	cSpaceShip* spaceShip4 = IoC.Resolve<cSpaceShip>("A", "cSpaceShip", std::string("SpaceShip #4"));
//
//	game1->Register(spaceShip1);
//	game1->Register(spaceShip2);
//	game2->Register(spaceShip3);
//	game2->Register(spaceShip4);
//
//	endPoint.Register(game1);
//	endPoint.Register(game2);
//	endPoint.set(IoC);
//
//	// load two command to different games
//	// moving direction for the first ship of the first game
//	TGameOperation<cVector> moveTo;
//	moveTo.gameId.id = "Game #1";
//	moveTo.objId.id = "SpaceShip #1";
//	moveTo.operationId.id = "moveTo";
//	moveTo.operationParameters = cVector(23, 45);
//
//	cMessage m1 = cMessage::Create(moveTo);
//	broker.put(m1);
//
//	// load bombs to the second ship of the second game
//	TGameOperation<cTanker> refuel;
//	refuel.gameId.id = "Game #2";
//	refuel.objId.id = "SpaceShip #4";
//	refuel.operationId.id = "refuel";
//	cTanker tanker; tanker.fuel = 37;
//	refuel.operationParameters = tanker;
//
//	cMessage m2 = cMessage::Create(refuel);
//	broker.put(m2);
//
//	cMessage m;
//	while (true == broker.get(m))
//		endPoint.process(m);
//
//	game1->detach();
//	game2->detach();
//
//	game1->play();
//    game2->play();
//
//	using namespace std::chrono_literals;
//	std::this_thread::sleep_for(5s);
//
//	std::shared_ptr<iCommand> softStopCmd1(new cSoftStopCommand(game1));
//	std::shared_ptr<iCommand> softStopCmd2(new cSoftStopCommand(game2));
//
//	game1->push_back(softStopCmd1);
//	game2->push_back(softStopCmd2);
//
//	const cObject* k = (*game1)["SpaceShip #1"];
//	cVector posSpaceShip1 = ((const cSpaceShip*)k)->Position();
//	EXPECT_EQ(23.0, posSpaceShip1.x);
//	EXPECT_EQ(45.0, posSpaceShip1.y);
//
//	const cObject* k2 = (*game2)["SpaceShip #4"];
//	auto fuel = ((const cSpaceShip*)k2)->Fuel();
//	EXPECT_EQ(25, fuel);
//}
